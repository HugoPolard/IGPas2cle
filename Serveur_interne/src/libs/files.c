//
// Created by gregoire on 08/04/2020.
//

#include <stdlib.h>
#include "files.h"

int get_lines_in_file(char *filename)
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
		return -1;
	int lines = 0;
	while (!feof(fp))
	{
		int ch = fgetc(fp);
		if (ch == '\n')
		{
			lines++;
		}
	}
	return lines;
}

char *readLastLine(char *filename)
{
	FILE *fd;
	char *buff = malloc(1024 * sizeof(char));
	if ((fd = fopen(filename, "r")) != NULL) // open file
	{
		fseek(fd, 0, SEEK_SET); // make sure start from 0
		while (!feof(fd))
		{
			fscanf(fd, "%[^\n]\n", buff); // read file *prefer using fscanf
		}
	}
	return buff;
}
