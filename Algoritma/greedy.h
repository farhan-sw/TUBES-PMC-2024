#ifndef GREEDY_H
#define GREEDY_H

#define MAX 255

// Fungsi untuk mencari indeks dalam matriks dari suatu kota
int search(char namakota[255], char kota[15][255], int *ind);

// Fungsi untuk menyelesaikan Travelling Salesman Problem dengan algoritma greedy
void travellingsalesman(int current_vertex, int starting_vertex, float matrix[15][15], int visited[10], int n, float *cost, char mat_kota[15][255]);

// Fungsi untuk menjalankan algoritma greedy
int greedy(char path_file[MAX], char startCity[MAX]);

#endif /* GREEDY_H */
