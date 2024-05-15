#ifndef BNB_H
#define BNB_H

#include <stdbool.h>

#define MAX 255
#define MAX_CITY 15

// Fungsi untuk mencari indeks kota dalam list
int findCityIndex(char listOfCities[MAX_CITY][MAX], char city[MAX]);

// Fungsi untuk mendapatkan nama kota berdasarkan indeks
char *getCityName(char listOfCities[MAX_CITY][MAX], int index);

// Fungsi rekursif untuk algoritma Branch and Bound
void branchAndBound(float adjacencyMatrix[MAX_CITY][MAX_CITY], int path[MAX_CITY], bool visited[MAX_CITY], int currentCity, int startCity, int numVertices, float cost, float *minCost);

// Fungsi untuk menjalankan algoritma Branch and Bound
int bnb(char path_file[MAX], char startCity[MAX]);

#endif /* BRANCH_AND_BOUND_H */
