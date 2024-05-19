#include "bnb.h"

/**
 *  ======================== ALGORITMA BRANCH AND BOUND ========================
 * Algoritma ini digunakan untuk menyelesaikan Travelling Salesman Problem (BNB_Main)
 * dengan menggunakan pendekatan Branch and Bound.
 * 
 * Pembuat: 13222114 / Muhammad Farhan
 * Last Modified: 2024-05-16
 * 
 * Referensi:
 * https://www.geeksforgeeks.org/traveling-salesman-problem-using-branch-and-bound-2/
 * https://github.com/matteosoo/Traveling-Salesman-Problem/blob/master/branch_and_bound.cpp
 * https://www.geeksforgeeks.org/travelling-salesman-problem-set
 * https://informatika.stei.itb.ac.id/~rinaldi.munir/Stmik/2021-2022/Makalah/Makalah-IF2211-Stima-2022-K1%20(47).pdf
*/

int isDebug = 0;


/* ======================== DEKLARASI VARIABEL GLOBAL ======================== */
int path_tmp[MAX_CITY + 1];
float final_res = FLT_MAX;
int visited[MAX_CITY];
int N;


/* ==================== DEKLARASI FUNGSI DAN PROSEDUR ==================== */
/**
 * @brief Copy path yang sedang dijalankan ke path final
 * @param curr_path Path yang sedang dijalankan
*/
void CopyPath(int curr_path[]) {
    for (int i = 0; i < N; i++)
        path_tmp[i] = curr_path[i];
    path_tmp[N] = curr_path[0];
}

/**
 * @brief Mencari nilai minimum pertama dari adjacency matrix
 * @param adj Adjacency matrix
 * @param i Indeks baris
 * @return Nilai minimum pertama
*/
float MinAdj_First(float adj[MAX_CITY][MAX_CITY], int i) {
    float min = FLT_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

/**
 * @brief Mencari nilai minimum kedua dari adjacency matrix
 * @param adj Adjacency matrix
 * @param i Indeks baris
 * @return Nilai minimum kedua
*/
float MinAdj_Sec(float adj[MAX_CITY][MAX_CITY], int i) {
    float first = FLT_MAX, second = FLT_MAX;
    for (int j = 0; j < N; j++) {
        if (i == j)
            continue;
        if (adj[i][j] <= first) {
            second = first;
            first = adj[i][j];
        } else if (adj[i][j] <= second && adj[i][j] != first)
            second = adj[i][j];
    }
    return second;
}

/**
 * @brief Fungsi rekursif untuk algoritma Branch and Bound
 * @param adj Adjacency matrix
 * @param curr_bound Batas saat ini
 * @param curr_weight Bobot saat ini
 * @param level Level saat ini
 * @param curr_path Path saat ini
*/
void BNB_Rekursif(float adj[MAX_CITY][MAX_CITY], float curr_bound, float curr_weight,
            int level, int curr_path[]) {
    if (level == N) {                                                       // Jika sudah mencapai level maksimal
        if (adj[curr_path[level - 1]][curr_path[0]] != 0) {                 // Jika kota terakhir terhubung dengan kota awal
            float curr_res = curr_weight +                                  // Menambahkan bobot
                adj[curr_path[level - 1]][curr_path[0]];
            if (curr_res < final_res) {                                     // Jika bobot saat ini lebih kecil dari bobot terakhir
                CopyPath(curr_path);                                     // Copy path saat ini ke path final
                final_res = curr_res;
            }
        }
        return;
    }
    for (int i = 0; i < N; i++) {
        
        if (adj[curr_path[level - 1]][i] != 0 &&                            // Jika kota i belum dikunjungi
            visited[i] == 0) {
            float temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];                    // Menambahkan bobot

            if (level == 1)                                                 // Mengurangi batas
                curr_bound -= ((MinAdj_First(adj, curr_path[level - 1]) +
                                MinAdj_First(adj, i)) / 2);
            else
                curr_bound -= ((MinAdj_Sec(adj, curr_path[level - 1]) +
                                MinAdj_First(adj, i)) / 2);
            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = 1;
                BNB_Rekursif(adj, curr_bound, curr_weight, level + 1,
                        curr_path);
            }

            curr_weight -= adj[curr_path[level - 1]][i];                        // Backtrack
            curr_bound = temp;                                                  // Reset batas
            memset(visited, 0, sizeof(visited));
            for (int j = 0; j <= level - 1; j++)                                // Reset path
                visited[curr_path[j]] = 1;
        }
    }
}

/**
 * @brief Fungsi utama untuk menjalankan algoritma Branch and Bound
 * @param adj Adjacency matrix
 * @param start_city Kota awal
*/
void BNB_Main(float adj[MAX_CITY][MAX_CITY], int start_city) {
    int curr_path[MAX_CITY + 1];   
    float curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < N; i++)                                                 // Menghitung batas
        curr_bound += (MinAdj_First(adj, i) + MinAdj_Sec(adj, i));                   
    curr_bound = (curr_bound < 1) ? 1 :                                         // Jika batas kurang dari 1, maka 1
                                    curr_bound / 2;                             // Jika tidak, dibagi 2
    visited[start_city] = 1;                                                    // Menandai kota awal           
    curr_path[0] = start_city;
    BNB_Rekursif(adj, curr_bound, 0, 1, curr_path);
}

// ======================== IMPLEMENTASI MAIN FUNCTION ========================
/**
 * @brief Fungsi untuk menjalankan algoritma Branch and Bound
 * @param path_file Path file yang berisi adjacency matrix
 * @param startCity Kota awal
 * @return 0 jika berhasil
*/
int bnb(char path_file[MAX_CHAR], char startCity[MAX_CHAR]) {
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    char cityName[MAX_CITY][MAX_CHAR];

    open_init(path_file, adjacencyMatrix, cityName, &N);                    // Membuka file dan menginisialisasi adjacency matrix

    int startCityIndex = isCityExist(cityName, startCity, N);               // Mencari index kota awal

    float adj[MAX_CITY][MAX_CITY];                                          // Copy adjacency matrix ke variabel lokal
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj[i][j] = adjacencyMatrix[i][j];
        }
    }

    if(isDebug){
    printf("N: %d\n", N);
    printf("Start City Index: %d\n", startCityIndex);}

    // Algoritma BNB
    BNB_Main(adj, startCityIndex);

    // Print Rute
    printf("Best route found: \n");
    for (int i = 0; i <= N; i++) {                                          // Print rute terbaik
        printf("%s", cityName[path_tmp[i]]);
        if (i != N) {                                                       // Jika bukan kota terakhir
            printf(" -> ");
        }
    }
    printf("\n");

    // Calculate Minimum Cost from path_tmp and costMatrix
    float minCost = 0;
    for (int i = 0; i < N; i++) {
        if(isDebug)
            printf("Cost from %d to %d: %.2f\n", path_tmp[i], path_tmp[i + 1], adj[path_tmp[i]][path_tmp[i + 1]]);
        minCost += adj[path_tmp[i]][path_tmp[i + 1]];
    }
    printf("Best route distance: %.5f km\n", minCost);
    
    return 0;
}
