#ifndef DFS_H
#define DFS_H

#include <stdbool.h>

#define MAX 255
#define INF 999999

// Fungsi untuk mengolah stack
void push(int newdat);
int pop();

// Fungsi untuk operasi matematis
int fac(int n);

// Fungsi untuk mencari indeks dari kota yang dicari
int find_city_index(char listOfCities[15][MAX], char city[MAX]);

// Fungsi untuk memeriksa apakah kota telah terdapat dalam stack visited
int not_on_stack(int kota);

// Fungsi untuk mengcopy jalur terpendek pada stack ke dalam array di main
int copy_stack(int shortestRoute[15], int n);

// Depth First Search Algorithm
void DFS_Algorithm(int currentCity, int lastCity, int destination, float adjacencyMatrix[15][15], int numVertices, 
                   int *depth, float *min, float *localDistance, int shortestRoute[15]);

// Fungsi untuk menjalankan algoritma Depth First Search
int dfs(char fileName[MAX], char startCity[MAX]);

#endif /* DFS_H */
