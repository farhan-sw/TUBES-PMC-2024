#include <stdio.h>

// Include Time
#include <time.h>

#include "utils\openFile.h"

/* Include Algorithm */
#include "algoritma\dfs.h"
#include "algoritma\bnb.h"
#include "algoritma\greedy.h"
#include "algoritma\bruteforce.h"

/* Include Utils */
#include "utils\openFile.h"

/* Include Constants */
#include "utils\constants.h"

int main() {
    /* Kode Akan menghitung Masing Masing Waktu untuk Menjalakan Alggoritma */
    clock_t start, end;
    double time_taken;

    /* Meminta Input Nama File dan Kota */
    char fileName[MAX_CHAR];
    char path[MAX_CHAR];
    char city[MAX_CITY][MAX_CHAR];
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    int numVertices;

    /* while Loop untuk meminta input nama file sampai benar */
    printf("Masukkan nama file: ");
    scanf("%s", fileName);

    /* Gabung path data dengan nama file */
    strcat(path, "data\\");
    strcat(path, fileName);

    while(open_init(path, adjacencyMatrix, city, &numVertices) == 0) {
        printf("Masukkan nama file: ");
        scanf("%s", fileName);

        /* Gabung path data dengan nama file */
        strcat(path, "data\\");
        strcat(path, fileName);
    }

    /* do-while Loop untuk meminta input kota sampai benar */
    char startCity[MAX_CHAR];
    int startCityIndex;
    do {
        printf("Masukkan kota awal: ");
        scanf("%s", startCity);

        startCityIndex = findCityIndex(city, startCity);

    } while (startCityIndex == -1);


    // Run DFS Algorithm
    printf("DFS Algorithm\n");
    // Jalankan timer
    start = clock();
    dfs(path, startCity);
    // Stop timer
    end = clock();
    // Hitung waktu yang dibutuhkan dalam detik
    time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("DFS Time: %.20f\n\n", time_taken);

    // Run Branch and Bound Algorithm
    printf("Branch and Bound Algorithm\n");
    // Jalankan timer
    start = clock();
    bnb(path, startCity);
    // Stop timer
    end = clock();
    // Hitung waktu yang dibutuhkan dalam detik
    time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("Branch and Bound Time: %.20f\n\n", time_taken);

    // Run Greedy Algorithm
    printf("Greedy Algorithm\n");
    // Jalankan timer
    start = clock();
    greedy(path, startCity);
    // Stop timer
    end = clock();
    // Hitung waktu yang dibutuhkan dalam detik
    time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("Greedy Time: %.20f\n\n", time_taken);

    puts("Brute Force Algorithm");
    start = clock();
    bruteForce(path, startCity);
    end = clock();

    time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("Brute Force Time: %.20f\n\n", time_taken);

    return 0;
}