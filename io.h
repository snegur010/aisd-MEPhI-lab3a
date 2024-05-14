#ifndef IO_H
#define IO_H

#include "table.h"

int getInteger(KeyType *d);
int getinteger(int *d);
char *readline(const char *str);
char* fread_line(FILE *fin);
#endif