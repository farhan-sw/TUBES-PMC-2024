#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <float.h>

#include "..\utils\openFile.h"

#define MAX 255
#define MAX_CITY 15

// Fungsi untuk mencari indeks kota dalam list
int findCityIndex(char listOfCities[MAX_CITY][MAX], char city[MAX]) {
    for (int j = 0; j < MAX_CITY; j++) {
        if (strcmp(city, listOfCities[j]) == 0) {
            return j;
        }
    }
    return -1; // Return -1 jika kota tidak ditemukan
}

// Fungsi untuk mendapatkan nama kota berdasarkan indeks
char *getCityName(char listOfCities[MAX_CITY][MAX], int index) {
    return listOfCities[index];
}

// Fungsi rekursif untuk algoritma Branch and Bound
void branchAndBound(float adjacencyMatrix[MAX_CITY][MAX_CITY], int path[MAX_CITY], bool visited[MAX_CITY], int currentCity, int startCity, int numVertices, float cost, float *minCost) {
    // Kasus Dasar: Semua kota telah dikunjungi
    if (currentCity == numVertices) {
        if (adjacencyMatrix[path[currentCity - 1]][startCity] != 0) {
            float currentCost = cost + adjacencyMatrix[path[currentCity - 1]][startCity];
            if (currentCost < *minCost) {
                *minCost = currentCost;
            }
        }
        return;
    }

    // Kasus Rekursif
    for (int i = 0; i < numVertices; i++) {
        if (adjacencyMatrix[path[currentCity - 1]][i] != 0 && !visited[i]) {
            visited[i] = true;
            path[currentCity] = i;
            branchAndBound(adjacencyMatrix, path, visited, currentCity + 1, startCity, numVertices, cost + adjacencyMatrix[path[currentCity - 1]][i], minCost);
            visited[i] = false;
        }
    }
}

int bnb(char path_file[MAX], char startCity[MAX]) {
    int path[MAX_CITY];
    bool visited[MAX_CITY];

    // Inisialisasi Variabel
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    char cityName[MAX_CITY][MAX];
    int numVertices;

    open_init(path_file, adjacencyMatrix, cityName, &numVertices);

    // Mencari Titik Mulai
    int startCityInd = findCityIndex(cityName, startCity);

    // Start dari hasil pencarian kota
    path[0] = startCityInd;
    visited[startCityInd] = true;

    // Inisiasi visited dari kota lainnya
    for (int i = 0; i < MAX_CITY; i++) {
        if (i != startCityInd) {
            visited[i] = false;
        }
    }

    float minCost = FLT_MAX; // Inisialisasi minimum cost dengan nilai yang sangat besar

    // Panggil fungsi Branch and Bound
    branchAndBound(adjacencyMatrix, path, visited, 1, startCityInd, numVertices, 0, &minCost);

    // Cetak path dengan menggunakan nama kota
    printf("Best route found:\n");
    for (int i = 0; i < numVertices; i++) {
        printf("%s", getCityName(cityName, path[i]));
        if (i != numVertices - 1) {
            printf(" -> ");
        }
    }
    printf(" -> %s\n", getCityName(cityName, startCityInd));

    // Cetak minimum cost
    printf("Best route distance: %.5f km\n", minCost);

    return 0;
}
