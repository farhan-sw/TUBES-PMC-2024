#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

// Definisikan maksimum jumlah kota
#define MAX_N 6

// Inisialisasi cost matrix
float costMatrix[MAX_N][MAX_N] = {
    {0.00, 838.63, 311.30, 567.66, 95.59, 120.16},
    {838.63, 0.00, 533.91, 284.52, 932.74, 942.62},
    {311.30, 533.91, 0.00, 256.85, 402.45, 409.18},
    {567.66, 284.52, 256.85, 0.00, 659.30, 665.04},
    {95.59, 932.74, 402.45, 659.30, 0.00, 46.79},
    {120.16, 942.62, 409.18, 665.04, 46.79, 0.00}
};

// #define MAX_N 4
// float costMatrix[MAX_N][MAX_N] = {
//     {0.00, 10.00, 15.00, 20.00},
//     {10.00, 0.00, 35.00, 25.00},
//     {15.00, 35.00, 0.00, 30.00},
//     {20.00, 25.00, 30.00, 0.00}
// };

// Jalur yang diambil
int bestPath[MAX_N];
// Jalur terpendek yang ditemukan
float minCost = FLT_MAX;

// Fungsi untuk mengecek apakah kota telah dikunjungi dalam jalur
int isVisited(int city, int path[], int size) {
    for (int i = 0; i < size; i++) {
        if (path[i] == city) {
            return 1;
        }
    }
    return 0;
}

// Fungsi untuk menghitung cost jalur
float calculateCost(int path[], int size) {
    float cost = 0.0;
    for (int i = 0; i < size - 1; i++) {
        cost += costMatrix[path[i]][path[i + 1]];
    }
    // Tambahkan cost dari kota terakhir kembali ke kota awal
    cost += costMatrix[path[size - 1]][path[0]];
    return cost;
}

// Fungsi rekursif untuk mencari jalur terpendek menggunakan Branch and Bound
void branchAndBound(int path[], int level, int size) {
    if (level == size) {
        float currentCost = calculateCost(path, size);
        // Update jalur terpendek
        if (currentCost < minCost) {
            minCost = currentCost;
            for (int i = 0; i < size; i++) {
                bestPath[i] = path[i];
            }
        }
        return;
    }

    for (int i = 0; i < size; i++) {
        // Jika kota belum dikunjungi
        if (!isVisited(i, path, level)) {
            path[level] = i;
            // Reduksi cost matrix
            float reducedCost = 0.0;
            for (int j = 0; j < size; j++) {
                if (!isVisited(j, path, level)) {
                    float min = FLT_MAX;
                    for (int k = 0; k < size; k++) {
                        if (k != i && costMatrix[k][j] < min) {
                            min = costMatrix[k][j];
                        }
                    }
                    reducedCost += min;
                }
            }
            // Jika penjumlahan cost tidak lebih besar dari minCost, maka jalur dieksplorasi
            if (minCost >= calculateCost(path, level) + reducedCost) {
                branchAndBound(path, level + 1, size);
            }
        }
    }
}

int main() {
    // Inisialisasi path
    int path[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        path[i] = -1;
    }

    // Input kota awal dari pengguna
    int startingCity;
    printf("Masukkan kota awal (0-%d): ", MAX_N - 1);
    scanf("%d", &startingCity);
    if (startingCity < 0 || startingCity >= MAX_N) {
        printf("Kota awal tidak valid!\n");
        return 1;
    }
    
    // Jalankan Branch and Bound
    branchAndBound(path, 0, MAX_N);

    // Atur jalur terpendek sesuai dengan kota awal yang dimasukkan pengguna
    int adjustedPath[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        adjustedPath[i] = (bestPath[i] + startingCity) % MAX_N;
    }

    // Print jalur terpendek dan costnya
    printf("Jalur Terpendek dari kota %d: ", startingCity);
    for (int i = 0; i < MAX_N; i++) {
        printf("%d ", adjustedPath[i]);
    }

    // Kalkulasi COst berdasarkan path dan costMatrix
    float cost = 0;
    for (int i = 0; i < MAX_N - 1; i++) {
        cost += costMatrix[adjustedPath[i]][adjustedPath[i + 1]];
    }
    cost += costMatrix[adjustedPath[MAX_N - 1]][adjustedPath[0]];
    printf("\nCost: %.2f\n", cost);

    return 0;
}
