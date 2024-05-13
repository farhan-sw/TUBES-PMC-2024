#ifndef openFile_H
#define openFile_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.1416
#define r 6371

int count_delim(char *string, char delim);
int open_init(char *namaFile, float matriks[15][15], char namaKota[15][255], int *n);

#endif /* openFile_H */
