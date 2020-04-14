//
// Created by gregoire on 09/04/2020.
//

#include "split.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>

char *split(char *string, char *delimiter, int num)
{
    size_t len = strlen(string);
    char *copy = malloc(sizeof(char) * (len + 1));
    memcpy(copy, string, len + 1); // pour avoir le \0
    if (num < 1)
    {
        log_warn("Le nombre minimum doit être 1");
        return NULL;
    }
    if (num == 1)
    {
        return strtok_r(copy, delimiter, &copy);
    }
    else
    {
        char *tok = NULL;
        for (int i = 1; i <= num; ++i)
        {
            tok = strtok_r(copy, delimiter, &copy);
        }
        if (tok != NULL)
        {
            return tok;
        }
    }
    log_warn("Impossible de trouver la partie de chaîne demandée");
    return NULL;
}
