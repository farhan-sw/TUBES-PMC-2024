#ifndef openFile_H
#define openFile_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "constants.h"

int isCityExist(char city[MAX_CITY][MAX_CHAR], char cityName[MAX_CHAR], int numVertices);
char* getCityName(char city[MAX_CITY][MAX_CHAR], int index);

int count_delim(char *string, char delim);
int open_init(char *namaFile, float matriks[15][15], char namaKota[15][255], int *n);

#endif /* openFile_H */
