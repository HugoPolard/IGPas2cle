//
// Created by gregoire on 09/04/2020.
//

#include "split.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>

char *split(char *string, char *delimiter, int num) {
    size_t len = strlen(string);
    char *copy = calloc(sizeof(char), (len + 1));
    char *saveptr = NULL;
    memcpy(copy, string, len + 1); // pour avoir le \0
    if (num < 1) {
        log_warn("Le nombre minimum doit être 1");
        free(copy);
        return NULL;
    }
    if (num == 1) {

        char *retval = strtok_r(string, delimiter, &saveptr);
        size_t size = strlen(retval) + 1;
        char *v = calloc(sizeof(char), size);
        memcpy(v, retval, size);
        memcpy(string, copy, len + 1);
        free(copy);
        return v;
    } else {
        char *tok = NULL;
        tok = strtok_r(string, delimiter, &saveptr);
        for (int i = 2; i <= num; ++i) {
            tok = strtok_r(NULL, delimiter, &saveptr);
        }
        if (tok != NULL) {
//            copy = (char *) realloc(copy, strlen(tok) + 1);
            size_t size = strlen(tok) + 1;
            char *v = calloc(sizeof(char), size);
            memcpy(v, tok, size);
            memcpy(string, copy, len + 1);
            free(copy);
            return v;
        }
    }
    log_warn("Impossible de trouver la partie de chaîne demandée");
    free(copy);
    return NULL;


}