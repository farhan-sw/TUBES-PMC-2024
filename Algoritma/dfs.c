#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "..\utils\openFile.h"
#include <time.h>


#define MAX 255
#define INF 999999

int visited_dfs[15];
int top_dfs = -1;

// Untuk mengolah stack
void push(int newdat){
    if (top_dfs == 15){
        printf("\nStack Overflow");
        return;
    }
    top_dfs ++;
    visited_dfs[top_dfs] = newdat;
}

int pop(){
    int popped_dat;
    if (top_dfs == -1){
        printf("\nData kosong");
        exit(1);
    }
    popped_dat = visited_dfs[top_dfs];
    visited_dfs[top_dfs] = '-';
    top_dfs --;
    return popped_dat;
}

// Untuk operasi matematis
int fac(int n){
    if (n == 1) {
      return 1;
    }
    return n * fac(n - 1);
}

// Untuk mencari indeks dari kota yang dicari
int find_city_index(char listOfCities[15][MAX], char city[MAX]){
    for (int j = 0; j < 15; j++){
        if (strcmp(city, listOfCities[j]) == 0){
            return j;
        }
    }
    // Bila kota yang diinginkan tidak terdapat pada list
    return -1;
}

// Untuk memeriksa apakah kota telah terdapat dalam stack visited_dfs
int not_on_stack(int kota){
    int temp = -1;
    for (int i = 0; i < top_dfs; i ++){
        if (visited_dfs[i] == kota){
            return 0;
        }
    }
    return 1;
}

// Untuk mengcopy jalur terpendek pada stack ke dalam array di main
int copy_stack(int shortestRoute[15], int n){
    for (int i = 0; i < n; i++){
        shortestRoute[i] = visited_dfs[i];
    }
    return 1;
}

// Depth First Search
void DFS_Algorithm(int currentCity, int lastCity, int destination,float adjacencyMatrix[15][15], int numVertices, 
         int *depth, float *min, float localDistance, int shortestRoute[16]) {
    // Memasukan kota yang sedang dikunjungi ke dalam stack dan juga menambah jaraknya dari kota sebelumnya
    push(currentCity);
    *depth = *depth + 1;
    // Jika telah sampai ke kota terakhir (sebelum kembali ke titik awal)
    if (*depth == numVertices){
        push(destination);
        // Pengecekan apakah total jarak untuk satu jalur lebih kecil dari total jarak sebelumnya
        float finalDistance = localDistance + adjacencyMatrix[currentCity][destination];
        if (finalDistance < *min){
            *min = finalDistance;
            copy_stack(shortestRoute, numVertices+1);
        }
        pop();
        // Jarak akhir dan perjalanan akan diakhiri dengan perjalanan ke kota awal
    };
    // Pencarian kota berikutnya yang akan dikunjungi
    for (int i = 0; i < numVertices; i++){
        // Mencari jalur yang belum dilewati
        if (not_on_stack(i) == 1 && i != currentCity){
            // Menghitung jarak perjalanan berikutnya, bila jarak sementara sebelum kota terakhir sudah lebih besar dari jarak sebelumnya-
            // Perjalanan tidak akan dilanjutkan
            float futureDistance = localDistance + adjacencyMatrix[currentCity][i];
            if (futureDistance <= *min){
                DFS_Algorithm(i, currentCity, destination, adjacencyMatrix, numVertices, depth, min, futureDistance, shortestRoute);
            }
        }
    }
    // Menghilangkan riwayat perjalanan sebelumnya (Proses backtracking DFS_Algorithm)
    pop();
    *depth -=1;
}

// Akan Menerima Input Karakter dari Main, karakter Nama File dan Nama Kota
int dfs(char fileName[MAX], char startCity[MAX]){

    // Inisiasi variable pokok
    float adjacencyMatrix[15][15];
    int numVertices;        
    char cityName[15][MAX];

    // Membuka file dan menyimpan jarak dalam graph
    open_init(fileName, adjacencyMatrix, cityName, &numVertices);

    int startCityInd = find_city_index(cityName, startCity);

    // Inisiasi variable yang akan diolah selama proses pencarian jalan
    int depth = 0;
    float min = INF;
    int shortestRoute[16];
    float localDistance = 0;

    // Pemanggilan DFS_Algorithm
    DFS_Algorithm(startCityInd, startCityInd, startCityInd, adjacencyMatrix, numVertices, &depth, &min, 0, shortestRoute);

    // Pengoutputan rute dan jarak terbaik untuk TSP
    printf("Best route found:\n%s", cityName[startCityInd]);
    for(int i = 1; i < numVertices+1; i++){
        printf(" -> %s", cityName[shortestRoute[i]]);
    }

    printf("\nBest route distance: %.5f km\n", min);

    return 0;
}
