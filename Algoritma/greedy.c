#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "openFile.h"

#define pi 3.1416

/* 
Dibuat oleh : Chobirudin Jamil Yudaprawira (13222113)
Dibuat pada : 11/05/2024
Deskripsi   : Implementasi algoritma greedy untuk soal 2 tubes PPMC - EL2208
Referensi   : 
https://www.tutorialspoint.com/data_structures_algorithms/travelling_salesman_problem.htm
https://www.geeksforgeeks.org/traveling-salesman-problem-tsp-implementation/
https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-clock-determine-processor-time
*/

void search(char namakota[255], char kota[15][255], int *ind){
    int i = 0;
    for(i = 0; i < 15; i++){
        if(strcmp(kota[i], namakota) == 0){
            *ind = i;
        }
    }
}

void travellingsalesman(int current_vertex, int starting_vertex, float matrix[15][15], int visited[10], int n, float *cost, char mat_kota[15][255]) {
   int k, adj_vertex = 999;
   float min = 9999999999;
   float unused1, unused2;
   
   visited[current_vertex] = 1;
   printf("%s -> ", mat_kota[current_vertex]);
   
   for(k = 0; k < n; k++) {
      if((current_vertex != k) && (visited[k] == 0)) {
         if(matrix[current_vertex][k] < min) {
            min = matrix[current_vertex][k];
            adj_vertex = k;
         }
      }
   }
   
   if(min != 999) {
      (*cost) = (*cost) + min;
   }
   if(adj_vertex == 999) {
      printf("%s", mat_kota[starting_vertex]);
      (*cost) = (*cost) + matrix[current_vertex][starting_vertex];
      return;
   }
   travellingsalesman(adj_vertex, starting_vertex, matrix, visited, n, cost, mat_kota);
}

int main(){
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

    printf("Enter list of cities file name: ");
    scanf("%s", filename);
    if(open_init(filename, matrix, namaKota, &n) == 0) {
        return 0;
    }

    printf("Enter starting point: ");
    scanf("%s", buff);
    search(buff, namaKota, &ind_kota);

    // for(i = 0; i < n; i++) {
    //     for(j = 0; j < n; j++) {
    //         printf("%f ", matrix[i][j]);
    //         }
    //     printf("\n");
    // }

    printf("Best route found: ");
    start_time = clock();
    travellingsalesman(ind_kota, ind_kota,matrix, visited, n, &cost, namaKota);
    end_time = clock();
    timedif = ((double) (end_time - start_time))/CLOCKS_PER_SEC;
    printf("\nBest route distance: ");
    printf("%f\n", cost);
    printf("Time elapsed: %.20f s\n", timedif);
    return 0;
}