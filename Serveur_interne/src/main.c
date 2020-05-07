#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "libs/files.h"
#include "libs/log.h"
#include "libs/split.h"
#include <curl/curl.h>
#include "libs/curlHelper.h"
#include "libs/httpd.h"
#include <sys/stat.h>


void *fileChangeHandler(void *args) {


    //Creation le répertoire d'échange de données si nécessaire
    struct stat st = {0};
    char datapath[100];
    strcpy(datapath, exchange_dir);
    strcat(datapath, exchange_file);
    log_debug("path = %s", datapath);
    if (stat(exchange_dir, &st) == -1) {
        umask(000);
        mkdir(exchange_dir, S_IRWXU | S_IRWXG | S_IRWXO);
        chown(exchange_dir, 0, 0);
    }
//    FILE *fd = fopen(datapath, "w");
//    if (fd == NULL) {
//        log_error("file non valide");
//    }
//    fclose(fd);
//    char *filename = exchange_file;
    char old_path[50] = "";
    do {

        char *lastLine = readLastLine(datapath);
        if (lastLine == NULL) {

            sleep(1);
            free(lastLine);
            continue;
        }
        char *strUID = split(lastLine, " ; ", 1);
        char *strPath = split(lastLine, " ; ", 2);

        char *uid = split(strUID, "=", 2);
        char *path = split(strPath, "=", 2);

        if (strcmp(old_path, path)) {
            log_debug("uid = %s", uid);
            log_debug("path = %s", path);
            strcpy(old_path, path);
            sendFile(server_addr, uid, path);
            free(strPath);
            free(strUID);
            free(uid);
            free(path);
            free(lastLine);
        }


        usleep(1000000);
    } while (1);

    pthread_exit(NULL);
}

void route() {
    ROUTE_START()

    ROUTE_GET("/door")
        {
            printf("HTTP/1.1 200 OK\r\n\r\n");

            log_debug("Query String");
            log_debug("%s", qs);
            char *param = split(qs, "=", 1);
            char *value = split(qs, "=", 2);
            if (strcmp(param, "open") == 0) {
                if (strcmp(value, "true") == 0) {
                    log_info("On ouvre la porte");
                    system("systemctl kill -s 10 doorD.service");
                } else {
                    log_info("On n'ouvre pas la porte");
                }
            }
            free(param);
            free(value);
        }
    ROUTE_GET("/lock")
        {
            printf("HTTP/1.1 200 OK\r\n\r\n");
            log_debug("Query String");
            log_debug("%s", qs);
            char *param = split(qs, "=", 1);
            char *value = split(qs, "=", 2);
            if (strcmp(param, "lock") == 0) {
                if (strcmp(value, "true") == 0) {
                    log_info("On verouille la porte");
                    system("systemctl kill -s SIGUSR2 doorD.service");
                } else {
                    log_info("On ne verouille pas la porte");
                }
            }
            free(param);
            free(value);

        }
        ROUTE_END()
}


int main(int argc, char **argv) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    log_set_level(4);
    parse_env();
    log_debug("%s\n%s\n%s\n", exchange_file, exchange_dir, server_addr);
    log_debug("Curl init réussie");
    pthread_t fileThread;
    pthread_create(&fileThread, NULL, fileChangeHandler, NULL);
    serve_forever("10000");
    pthread_join(fileThread, NULL);
    log_info("A bientôt");

    return 0;
}
