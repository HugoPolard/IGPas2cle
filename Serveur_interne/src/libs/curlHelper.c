//
// Created by gregoire on 09/04/2020.
//

#include "curlHelper.h"
#include <sys/stat.h>
#include <curl/curl.h>
#include "log.h"

int sendFile(const char *url,const char * uid, const char *path)
{
    FILE *fd;
    CURL *curl;
    CURLcode res;
    struct stat file_info;
    curl_mime *form = NULL;
    curl_mimepart *field = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";

    fd = fopen(path, "rb");
    if (fd == NULL)
    {
        log_error("Fichier non trouvé");
        return 1;
    }
    if (fstat(fileno(fd), &file_info) != 0)
    {
        log_error("Fichier non valide");
    }
    curl = curl_easy_init();
    if (curl)
    {
        form = curl_mime_init(curl);

        field = curl_mime_addpart(form);
        curl_mime_name(field, "image");
        curl_mime_filedata(field, path);
        field = curl_mime_addpart(form);
        curl_mime_name(field, "filename");
        curl_mime_data(field, uid, CURL_ZERO_TERMINATED);

        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/projet/index.php");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        
        FILE *f = fopen("curl.log", "wb");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);


        headerlist = curl_slist_append(headerlist, buf);
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            log_error("curl a échoué : %s", curl_easy_strerror(res));
        }
        else
        {
            log_info("Image transférée");
        }
        /* always cleanup */
        curl_easy_cleanup(curl);

        curl_mime_free(form);
        /* free slist */
        curl_slist_free_all(headerlist);
    }
    fclose(fd);
    return 0;
}
