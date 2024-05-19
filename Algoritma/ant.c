/*
Nama/NIM       : Nurul Annisa/13222109
Deskripsi      : Pengerjaan Tugas Besar PPMC EL2208 Soal Nomor 2 dengan Algoritma Ant Colony Optimization
Referensi      : 
https://medium.com/thelorry-product-tech-data/ant-colony-optimization-to-solve-the-travelling-salesman-problem-d19bd866546e
https://www.geeksforgeeks.org/introduction-to-ant-colony-optimization/
file:///C:/Users/User/Downloads/193-Article%20Text-986-1-10-20200511.pdf                
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "..\utils\openFile.h" // Mengimpor header file untuk fungsi open_init

#define ALPHA 1         // Faktor jalur feromon
#define BETA 2          // Faktor informasi heuristik
#define RHO 0.5         // Laju penguapan feromon
#define Q 100           // Faktor deposit feromon
#define MAX_ITER 1000   // Jumlah iterasi maksimum
#define NUM_ANTS 10     // Jumlah semut
#define MAX 255

int size; // Menyimpan jumlah kota

// Fungsi untuk optimasi ACO
void antColonyOptimization(float matriks[15][15], float **pheromones, float **deltaPheromones, int *bestTour, float *bestTourLength) {
    // Alokasi memori untuk variabel yang diperlukan
    float *antDistances = (float*)malloc(NUM_ANTS * sizeof(float));
    float *pheromoneInfluence = (float*)malloc(size * sizeof(float));
    int *visited = (int*)malloc(size * sizeof(int));
    int *tour = (int*)malloc(size * sizeof(int));

    // Loop iterasi maksimum
    for (int iteration = 0; iteration < MAX_ITER; iteration++) {
        // Loop untuk setiap semut
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            // Inisialisasi
            memset(visited, 0, size * sizeof(int));
            tour[0] = rand() % size;
            visited[tour[0]] = 1;

            // Loop untuk setiap langkah semut
            for (int step = 1; step < size; step++) {
                // Menghitung pengaruh feromon
                float total = 0.0;
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        pheromoneInfluence[i] = pow(pheromones[tour[step - 1]][i], ALPHA) * pow(1.0 / matriks[tour[step - 1]][i], BETA);
                        total += pheromoneInfluence[i];
                    } else {
                        pheromoneInfluence[i] = 0.0;
                    }
                }

                // Seleksi roda roulette
                float rnd = (float)rand() / RAND_MAX * total;
                float cumulative = 0.0;
                int next = -1;
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        cumulative += pheromoneInfluence[i];
                        if (cumulative >= rnd) {
                            next = i;
                            break;
                        }
                    }
                }

                if (next == -1) {
                    // Jika tidak ada kota yang terpilih, pilih kota acak
                    for (int i = 0; i < size; i++) {
                        if (!visited[i]) {
                            next = i;
                            break;
                        }
                    }
                }

                tour[step] = next;
                visited[next] = 1;
            }

            // Menghitung jarak tur
            float distance = 0.0;
            for (int i = 0; i < size - 1; i++) {
                distance += matriks[tour[i]][tour[i + 1]];
            }
            distance += matriks[tour[size - 1]][tour[0]]; // Melengkapi tur

            antDistances[ant] = distance;

            // Memperbarui tur terbaik
            if (distance < *bestTourLength || *bestTourLength == -1) {
                *bestTourLength = distance;
                memcpy(bestTour, tour, size * sizeof(int));
            }
        }

        // Memperbarui delta feromon
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                deltaPheromones[i][j] = 0;
            }
        }
        for (int k = 0; k < NUM_ANTS; k++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    deltaPheromones[i][j] += Q / antDistances[k];
                }
            }
        }

        // Memperbarui feromon
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                pheromones[i][j] = (1 - RHO) * pheromones[i][j] + deltaPheromones[i][j];
            }
        }
    }

    // Membebaskan memori yang dialokasikan
    free(antDistances);
    free(pheromoneInfluence);
    free(visited);
    free(tour);
}

// Fungsi utama untuk menginisialisasi dan menjalankan optimasi koloni semut
int ant(char path_file[MAX], char startCity[MAX]) {
    char namaFile[255];
    float matriks[15][15];
    int n = 0;
    char namaKota[15][255];

    // Membuka dan menginisialisasi data dari file
    open_init(path_file, matriks, namaKota, &n);

    size = n; // Mengatur ukuran sesuai dengan jumlah kota yang dibaca

    // Inisialisasi feromon
    float **pheromones = (float**)malloc(size * sizeof(float*));
    float **deltaPheromones = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        pheromones[i] = (float*)malloc(size * sizeof(float));
        deltaPheromones[i] = (float*)malloc(size * sizeof(float));
        for (int j = 0; j < size; j++) {
            pheromones[i][j] = 1.0; // Level feromon awal
            deltaPheromones[i][j] = 0.0;
        }
    }

    int *bestTour = (int*)malloc(size * sizeof(int));
    float bestTourLength = -1;

    // Mencari indeks kota awal
    int startingIndex;
    for (int i = 0; i < size; i++) {
        if (strcmp(startCity, namaKota[i]) == 0) {
            startingIndex = i;
            break;
        }
    }

    // Menjalankan optimasi koloni semut
    srand(time(NULL)); // Inisialisasi seed random
    antColonyOptimization(matriks, pheromones, deltaPheromones, bestTour, &bestTourLength);
    
    // Menemukan indeks dari tur terbaik
    int *bestTourIndices = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        bestTourIndices[i] = bestTour[i];
    }
    
    // Menemukan indeks kota awal dalam tur terbaik
    int startIndexInTour = -1;
    for (int i = 0; i < size; i++) {
        if (bestTour[i] == startingIndex) {
            startIndexInTour = i;
            break;
        }
    }

    // Mencetak rute terbaik yang ditemukan
    printf("Best route found:\n");
    for (int i = 0; i < size; i++) {
        printf("%s -> ", namaKota[bestTourIndices[(startIndexInTour + i) % size]]);
    }

    printf("%s\n", namaKota[bestTourIndices[startIndexInTour]]); // Mencetak kota awal lagi untuk melengkapi tur

    printf("Best route distance: %.5lf km\n", bestTourLength);

    // Membebaskan memori yang dialokasikan
    free(bestTourIndices);
    for (int i = 0; i < size; i++) {
        free(pheromones[i]);
        free(deltaPheromones[i]);
    }
    free(pheromones);
    free(deltaPheromones);
    return 0;
}
