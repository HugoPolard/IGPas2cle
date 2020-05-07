//
// Created by gregoire on 08/04/2020.
//

#include <stdlib.h>
#include "files.h"
#include "split.h"

int get_lines_in_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return -1;
    int lines = 0;
    while (!feof(fp)) {
        int ch = fgetc(fp);
        if (ch == '\n') {
            lines++;
        }
    }
    fclose(fp);
    return lines;
}

char *readLastLine(char *filename) {
    FILE *fd;
    char *buff = malloc(1024 * sizeof(char));
    if ((fd = fopen(filename, "r")) != NULL) // open file
    {
        fseek(fd, 0, SEEK_END); // make sure start from 0
        unsigned long len = (unsigned long) ftell(fd);
        if (len > 0) {
            fseek(fd, 0, SEEK_SET);
            while (!feof(fd)) {
                fscanf(fd, "%[^\n]\n", buff); // read file *prefer using fscanf
            }
        } else {
            free(buff);
            return NULL;
        }
    }
    fclose(fd);
    return buff;
}

void parse_env() {
    FILE *fd = fopen(".env", "r");
    char *buff = malloc(1024 * sizeof(char));
    if (fd != NULL) {
        fseek(fd, 0, SEEK_SET);
        while (!feof(fd)) {
            fscanf(fd, "%[^\n]\n", buff);

            char *field = split(buff, "=", 1);
            char *value = split(buff, "=", 2);

            if (strcmp(field, "EXCHANGE_FILE") == 0) {
                exchange_file = value;
            } else if (strcmp(field, "SERVER_URL") == 0) {
                server_addr = value;
            } else if (strcmp(field, "EXCHANGE_DIR") == 0) {
                exchange_dir = value;
            }
        }
    }
    fclose(fd);
    free(buff);
}
