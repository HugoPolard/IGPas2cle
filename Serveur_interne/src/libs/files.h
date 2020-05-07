//
// Created by gregoire on 08/04/2020.
//

#ifndef SERVEUR_INTERNE_FILES_H
#define SERVEUR_INTERNE_FILES_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>

int get_lines_in_file(char *filename);

char *readLastLine(char *filename);


// fichier .env
char *exchange_file;
char *server_addr;
char *exchange_dir;

void parse_env();

#endif //SERVEUR_INTERNE_FILES_H
