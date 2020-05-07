#include "getUID.h"
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <syslog.h>

#define NFCD_CONF "/etc/nfcd.conf"

int use_vblock;
char exchange_dir[100];
char exchange_file[100];

// Fonction de lecture du fichier de configuration pour le service nfcd
int get_nfcd_conf() {
	FILE* f = fopen(NFCD_CONF, "r");
	if (f == NULL)
		return 1;
	fscanf(f, "exchange_dir=%s\n", exchange_dir);
	fscanf(f, "exchange_file=%s", exchange_file);
	return 0;
}

// Fonction qui permet d'attendre une carte et lire son uid
char* get_card_uid(char* sn_str) {
	int 	tmp;
	char    *p;         // to hold [serial] as a string
	
	/*  Attente d'une lecture de carte OK
	*		Attente d'une carte de type précis (voir conf)
	*		Récupération du serial number et type de la carte
	*		Sélection de la carte et donc lecture de son UID pour entamer la communication
	*/
	while (get_card_info() != TAG_OK) usleep(5000);

	// transformation de l'uid en un chaine de caractères réutilisable
	p=sn_str;                       
	for (tmp=0;tmp<SN_len;tmp++) {  // on parcourt le Serial Number
		sprintf(p,"%02x",SN[tmp]);	// changement de format
		p+=2;
	}
	*(p++)=0;
	
	return sn_str;
}

// Fonction d'envoi des informations de la carte et photo au serveur local
void send_infos_to_server (char* uid_string, char* pic_path) {
	char path[100] = "", log_string[100] = "";
	// Création de la chaine contenant le chemin absolu du fichier d'échange
	sprintf(path, "%s%s", exchange_dir, exchange_file);
	FILE* f = fopen(path, "w");
	// Ecriture des données dans le fichier d'échange
	fprintf(f, "uid=%s; pic_path=%s;\n", uid_string, pic_path);
	// Ecriture de la chaine dans les logs du service pour garder une trace
	sprintf(log_string, "Chaine envoyee : uid= %s; pic_path=%s; vers %s\n", uid_string, pic_path, path);
	syslog(LOG_NOTICE, log_string);
	fclose(f);
	return;
}

int main(int argc, char *argv[]) {

	// Préparation pour pouvoir être lancé en tant que service
	pid_t pid, sid;
	pid = fork();
	if (pid < 0) {
	        exit(EXIT_FAILURE);
	}
	if (pid > 0) {
	        exit(EXIT_SUCCESS);
	}
	umask(0);
	sid = setsid();		// recréation d'une session indépendante
	if (sid < 0) {
    	exit(EXIT_FAILURE);
	}
	if ((chdir("/")) < 0) { 	// déplacement à la racine de cette session
    	exit(EXIT_FAILURE);
	}
	// Fermeture des buffers d'entrée/sorite classiques
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// must be run as root to open /dev/mem in BMC2835
    if (geteuid() != 0)
    {
        syslog(LOG_NOTICE, "Must be run as root.\n");
        exit(1);
    }

    // gestion des signaux pour la librairie
	set_signals();
	    
    /* lire /etc/rc522.conf */
    if (get_config_file()) exit(1);

    /* lire /etc/nfcd.conf 
	* permet d'obtenir le répertoire et fichier d'échange
    */
    if (get_nfcd_conf()) exit(1);

    /* Initialiser le BCM2835 pour pouvoir utiliser le bus SPI */
    if (HW_init(spi_speed,gpio)) close_out(1);
    
    /* initialiser le RC522 en fonction de la config lue plus tôt */
	InitRc522();

    /* read & set GID and UID from config file */
    if (read_conf_uid()!= 0) close_out(1);

	// Creation du répertoire d'échange de données si nécessaire
	struct stat st = {0};
	if (stat(exchange_dir, &st) == -1) {
		umask(000);
	    mkdir(exchange_dir, 0777);
	    chown(exchange_dir, 0, 0);
	}
	
    while (loop > 0)
    {
		char uid_string[23];		
		char filename[1000], command[1000];
		char log_string[100] = "";
		
		// Attente de la carte et lecture de l'uid
		get_card_uid(uid_string);

		// Création du timestamp pour la photo prise de l'entrée de la porte
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		sprintf(filename, "%s%d-%02d-%02d_%02d:%02d:%02d.jpg", exchange_dir, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		// Appel de fswebcam pour prendre la photo
		sprintf(command, "fswebcam %s", filename);
		sprintf(log_string, "commande : %s", command);
		system(command);
		// Trace de l'exécution de la commande
		syslog(LOG_NOTICE, log_string);
		// Envoi des infos au serveur RPI via le fichier d'échange
		send_infos_to_server(uid_string, filename);
        
        // Fermeture de la communication avec la carte NFC et reset du composant
        PcdHalt();                    
        sleep(3);
        
    }   // loop

    close_out(0);
    
    // stop -Wall complaining
    exit(0);
}
