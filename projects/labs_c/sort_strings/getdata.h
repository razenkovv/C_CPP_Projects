#ifndef GETDATA_H
#define GETDATA_H

#include <stdio.h>

char *get_str();
char **get_data(int *amount);
char *fget_str(FILE *in);
char **fget_data(FILE *in, int *amount);
int scan_choice(int n);
FILE *open_file_out();
FILE *open_file_in();

#endif
