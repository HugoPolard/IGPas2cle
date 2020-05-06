#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
#include <wiringPi.h>


#define	LED 29
#define	LED2 28
#define CHECK(sts,msg) if ((sts) == -1) { perror(msg); exit(-1); }
void deroute(int sig) 
{ 
    switch(sig){
    	case SIGUSR1 :
    		digitalWrite (LED, LOW) ;
    		sleep(5); 
    		digitalWrite (LED, HIGH) ;
    		syslog(LOG_NOTICE, "On ouvre la porte");
			break;
	case SIGUSR2 :
    		digitalWrite (LED2, LOW) ;
    		sleep(5); 
    		digitalWrite (LED2, HIGH) ;
    		syslog(LOG_NOTICE, "On refuse l'acces'");
			break;
	
    } 
} 

int main(void) {
        
        
        pid_t pid, sid;
        
        
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        umask(0);
        
        sid = setsid();
        if (sid < 0) {
                
                exit(EXIT_FAILURE);
        }
        
        if ((chdir("/")) < 0) {
                
                exit(EXIT_FAILURE);
        }
        
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        
        /* Daemon-specific initialization goes here */
        struct sigaction newact;
        newact.sa_handler = deroute;
	newact.sa_flags = SA_RESTART;
	
	wiringPiSetup () ;
  	pinMode (LED, OUTPUT) ;
  	pinMode (LED2, OUTPUT) ;
  
	CHECK(sigaction(SIGUSR1, &newact, NULL), "problème avec sigaction");
	CHECK(sigaction(SIGUSR2, &newact, NULL), "problème avec sigaction");
        /* The Big Loop */
        while (1) {
           
        }
   exit(EXIT_SUCCESS);
}
