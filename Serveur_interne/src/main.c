#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "libs/files.h"
#include "libs/log.h"
#include "libs/split.h"
#include <curl/curl.h>
#include <sys/stat.h>
#include "libs/curlHelper.h"



void *fileChangeHandler(void *args)
{

    //    char *filename = "/tmp/data_exchange/to_server.txt";
    char *filename = "test.txt";
    int old_lines = get_lines_in_file(filename);

    do
    {
        int newLines = get_lines_in_file(filename);
        if (newLines == -1)
        {
            log_fatal("Fichier non trouvé");
            break;
        }

        if (newLines >= old_lines)
        {
            char *lastLine = readLastLine(filename);
            char *strUID = split(lastLine, " ; ", 1);
            char *strPath = split(lastLine, " ; ", 2);

            char *uid = split(strUID, "=", 2);
            char *path = split(strPath, "=", 2);

            log_debug("uid = %s", uid);
            log_debug("path = %s", path);

            sendFile("http://localhost/projet/index.php", uid,path);
        }

        old_lines = newLines;

        usleep(1000000);
    } while (1);

    pthread_exit(NULL);
}



int main(int argc, char **argv)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    log_debug("Curl init réussie");
    pthread_t fileThread;
    pthread_create(&fileThread, NULL, fileChangeHandler, NULL);
    pthread_join(fileThread, NULL);
    log_info("A bientôt");
    return 0;
}
