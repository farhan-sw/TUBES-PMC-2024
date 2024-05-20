#ifndef ILP_H
#define ILP_H

#include <stdio.h>
#include <stdlib.h>
#include <glpk.h>
#include <math.h>
#include <string.h>
#include <time.h>

// Konstanta
#define MAX 255
#define INF 999999

// Fungsi
// Fungsi cari index
int find_city_index(char listOfCities[15][MAX], char city[MAX]);
// Fungsi ILP
int ilp(char path_file[MAX], char startCity[MAX]);

#endif // ILP_H
