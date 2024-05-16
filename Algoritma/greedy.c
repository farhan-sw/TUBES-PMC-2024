#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "..\utils\openFile.h"

#include "greedy.h"

/* 
Dibuat oleh : Chobirudin Jamil Yudaprawira (13222113)
Deskripsi   : Implementasi algoritma greedy untuk soal 2 tubes PPMC - EL2208
Referensi   : 
https://www.tutorialspoint.com/data_structures_algorithms/travelling_salesman_problem.htm
https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-clock-determine-processor-time
*/

// Fungsi untuk mencari indeks dalam matriks dari suatu kota
int search(char namakota[255], char kota[15][255], int *ind){
    int i;
    for(i = 0; i < 15; i++){
        if(strcmp(kota[i], namakota) == 0){
            *ind = i;
            return 0;
        }
    }

    // Jika tidak ditemukan
    return -1;
}

// Fungsi untuk menyelesaikan Travelling Salesman Problem dengan algoritma greedy
void travellingsalesman(int current_vertex, int starting_vertex, float matrix[15][15], int visited[10], int n, float *cost, char mat_kota[15][255]) {
   int k, adj_vertex = 999;
   float min = 9999999999;                                          // Diasumsikan jarak antarkota tidak ada yang melebihi ini
   
   visited[current_vertex] = 1;                                     // Menandai kota yang telah dikunjungi
   printf("%s -> ", mat_kota[current_vertex]);                      // Menampilkan kota yang sedang dikunjungi
   
   for(k = 0; k < n; k++) {                                         // Memeriksa semua kota yang belum dikunjungi dan mencari jarak minimum
      if((current_vertex != k) && (visited[k] == 0)) {
         if(matrix[current_vertex][k] < min) {
            min = matrix[current_vertex][k];
            adj_vertex = k;
         }
      }
   }
   
   if(min != 9999999999) {                                           // Jika ada jarak minimum cost/jarak ditambahkan
      (*cost) = (*cost) + min;
   }
   if(adj_vertex == 999) {
      printf("%s", mat_kota[starting_vertex]);                      // Mencetak kota awal jika semua kota sudah dikunjungi
      (*cost) = (*cost) + matrix[current_vertex][starting_vertex];  // Mencetak jarak kota awal ke kota awal
      return;
   }
   travellingsalesman(adj_vertex, starting_vertex, matrix, visited, n, cost, mat_kota); // Rekursif
}

int greedy(char path_file[MAX], char startCity[MAX]){

    // Inisialisasi
    char filename[255], buff[255], namaKota[15][255];
    int n = 0, i = 0, j = 0, ind_kota;

    float matrix[15][15], cost = 0;
    int visited[15];

    for(i = 0; i < 15; i++) {
        for(j = 0; j < 15; j++) {
            matrix[i][j] = 0;
        }
        visited[i] = 0;
    }

    open_init(path_file, matrix, namaKota, &n);
    search(startCity, namaKota, &ind_kota);

    printf("Best route found:\n");                                                     // Waktu mulai menjalankan algoritma
    travellingsalesman(ind_kota, ind_kota,matrix, visited, n, &cost, namaKota); // Memanggil algoritma greedy               // Menghitung selisih waktu
    printf("\nBest route distance: ");                                          // Mencetak output pada layar
    printf("%.5f km\n", cost);
    return 0;                                                                   // Program berakhir
}
