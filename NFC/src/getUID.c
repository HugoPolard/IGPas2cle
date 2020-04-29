#include "getUID.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define EXCHANGE_DIR "/tmp/data_exchange/"
#define EXCHANGE_FILE "to_server.txt"

int use_vblock;

char* get_card_uid(char* sn_str) {
	int 	tmp;
	char    *p;         // to hold [serial] as a string
	
	// wait for card
	while (get_card_info() != TAG_OK) usleep(5000);

	// create and display serial string
	p=sn_str;                       
	for (tmp=0;tmp<SN_len;tmp++) {  // add serial number
		sprintf(p,"%02x",SN[tmp]);
		p+=2;
	}
	*(p++)=0;
	
	// display card information
	if (debug)
		p_printf(GREEN,"Tag: type: %04x SNlen: %d SN: %s maxblocks: %d\n",CType,SN_len,sn_str, max_blocks);
	return sn_str;
}

void send_infos_to_server (char* uid_string, char* pic_path) {
	char path[100] = EXCHANGE_DIR;
	strcat(path, EXCHANGE_FILE);
	FILE* f = fopen(path, "w");
	printf("uid= %s; pic_path=%s; vers %s\n", uid_string, pic_path, path);
	fprintf(f, "uid=%s; pic_path=%s;\n", uid_string, pic_path);
	fclose(f);
	return;
}

int main(int argc, char *argv[]) {

	// must be run as root to open /dev/mem in BMC2835
    if (geteuid() != 0)
    {
        p_printf(RED,"Must be run as root.\n");
        exit(1);
    }

    // catch signals
	set_signals();
	    
    /* read /etc/rc522.conf */
    if (get_config_file()) exit(1);

    /* set BCM2835 Pins correct */
    if (HW_init(spi_speed,gpio)) close_out(1);
    
    /* initialise the RC522 */
	InitRc522();

    /* read & set GID and UID from config file */
    if (read_conf_uid()!= 0) close_out(1);
	
    while (loop > 0)
    {
		char uid_string[23];		
		char filename[1000], command[1000];

		// Creation le répertoire d'échange de données si nécessaire
		struct stat st = {0};
		char path[100] = EXCHANGE_DIR;
		strcat(path, EXCHANGE_FILE);
		if (stat(EXCHANGE_DIR, &st) == -1) {
		    mkdir(EXCHANGE_DIR, 0722);
		}
		
		// Attente de la carte et lecture de l'uid
		get_card_uid(uid_string);

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		sprintf(filename, "%s%d-%02d-%02d_%02d:%02d:%02d.jpg", EXCHANGE_DIR, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		sprintf(command, "fswebcam %s", filename);
		printf("cmd : %s\n", command);
		system(command);
		send_infos_to_server(uid_string, filename);
        // close / disconnect the card
        PcdHalt();                    
        sleep(3);
        
    }   // loop

    close_out(0);
    
    // stop -Wall complaining
    exit(0);
}
