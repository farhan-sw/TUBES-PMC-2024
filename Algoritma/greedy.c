#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "openFile.h"

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
   
   if(min != 999) {                                                 // Jika ada jarak minimum cost/jarak ditambahkan
      (*cost) = (*cost) + min;
   }
   if(adj_vertex == 999) {
      printf("%s", mat_kota[starting_vertex]);                      // Mencetak kota awal jika semua kota sudah dikunjungi
      (*cost) = (*cost) + matrix[current_vertex][starting_vertex];  // Mencetak jarak kota awal ke kota awal
      return;
   }
   travellingsalesman(adj_vertex, starting_vertex, matrix, visited, n, cost, mat_kota); // Rekursif
}

int main(){
    // Inisialisasi
    clock_t start_time, end_time;
    double timedif;
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

    // Input nama file
    printf("Enter list of cities file name: ");
    scanf("%s", filename);

    // Membuka file dan menyimpan jarak dalam matrix
    while(open_init(filename, matrix, namaKota, &n) == 0) {
        printf("\nEnter list of cities file name: ");
        scanf("%s", filename);
    }

    // Input kota awal
    printf("Enter starting point: ");
    scanf("%s", buff);

    while(search(buff, namaKota, &ind_kota) == -1) {
        printf("\n%s coordinates is not available in the database! \nEnter starting point: ", buff);
        scanf("%s", buff);
    }

    printf("Best route found: ");
    start_time = clock();                                                       // Waktu mulai menjalankan algoritma
    travellingsalesman(ind_kota, ind_kota,matrix, visited, n, &cost, namaKota); // Memanggil algoritma greedy
    end_time = clock();                                                         // Waktu selesai menjalankan algoritma
    timedif = ((double) (end_time - start_time))/CLOCKS_PER_SEC;                // Menghitung selisih waktu
    printf("\nBest route distance: ");                                          // Mencetak output pada layar
    printf("%f\n", cost);
    printf("Time elapsed: %.20f s\n", timedif);
    return 0;                                                                   // Program berakhir
}
