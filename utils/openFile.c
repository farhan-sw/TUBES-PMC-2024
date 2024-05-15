#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define pi 3.1416
#define r 6371
//#define MAX 255

int count_delim(char *string, char delim){
    int n = 0;
    for (int i = 0; i < strlen(string); i++){
        if (string[i] == delim){
            n += 1;
        }
    }
    return n;
}

int open_init(char *namaFile, float matriks[15][15], char namaKota[15][255], int *n){
    *n = 0;
    // Pembukaan dan pengecekan keberadaan file
    FILE *file = fopen(namaFile, "r");
    if (file == NULL){
        printf("\nFile tidak ada!\n");
        return 0;
    }
    // Inisiasi variable lokal yang akan digunakan untuk fungsi open_init
    char line[255];
    float latitude[15];
    float longitude[15];
    // Pembacaan file
    while (fgets(line, 255, file)){
        // Pengecekan kebenaran format file, untuk soal ini kami memutuskan untuk menggunakan delimiter berupa koma
        if (count_delim(line, ',') != 2){
            printf("\nTerdapat kesalahan format pada file!");
            return 0;
        }
        // Pemasukan data dari baris yang terbaca ke dalam masing masing variable dengan tipe data sesuai
        strcpy(namaKota[*n], strtok(line, ","));
        latitude[*n] = atof(strtok(NULL, ","));
        longitude[*n] = atof(strtok(NULL, "\n"));
        
        // printf("\n%s %f %f", namaKota[*n], latitude[*n], longitude[*n]);
        *n += 1;
    }
    
    // Pengecekan apakah jumlah kota sesuai ketentuan (6 <= jumlah kota <= 15)
    if (*n < 5 || *n > 14){
        printf("\nJumlah kota tidak sesuai!");
        return 0;
    }
    // Pengisian adjecent matriks, yang berisi jarak tiap titik ke titik lainnya
    for (int i = 0; i < *n; i++){
        for (int j = 0; j < *n; j++){
            // Komponen 1 : kolom, komponen 2 : baris
            matriks[i][j] = 2 * r * asin(sqrt(pow(sin((latitude[j]-latitude[i])*pi/360), 2) + cos(latitude[j]*pi/180) * cos(latitude[i]*pi/180) * pow(sin((longitude[j]-longitude[i])*pi/360), 2)));
        }
    }
    return 1;
}

// // Untuk testing
// int main(){
//     float matriks[15][15];
//     int n = 0;
//     char namaKota[15][255];
//     char namaFile[255];
//     scanf("%s", namaFile);

//     if (open_init(namaFile, matriks, namaKota, &n) == 0){
//         return 0;
//     }
//     printf("\n");
//     for (int i = 0; i < n; i ++){
//         for (int j = 0; j < n; j++){
//             printf("%f ", matriks[i][j]);
//         }
//         printf("\n");
//     }
// }
