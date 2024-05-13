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
    FILE *file = fopen(namaFile, "r");
    if (file == NULL){
        printf("\nFile tidak ada!");
        return 0;
    }
    
    char line[255];
    float latitude[15];
    float longitude[15];

    while (fgets(line, 255, file)){
        if (count_delim(line, ',') != 2){
            printf("\nTerdapat kesalahan format pada file!");
            return 0;
        }
        strcpy(namaKota[*n], strtok(line, ","));
        latitude[*n] = atof(strtok(NULL, ","));
        longitude[*n] = atof(strtok(NULL, "\n"));
        printf("\n%s %f %f", namaKota[*n], latitude[*n], longitude[*n]);
        *n += 1;
    }

    for (int i = 0; i < *n; i++){
        for (int j = 0; j < *n; j++){
            // Komponen 1 : kolom, komponen 2 : baris
            matriks[i][j] = 2 * r * asin(sqrt(pow(sin((latitude[i]-latitude[j])*pi/360),2) + cos(longitude[i]*pi/180)*cos(longitude[j]*pi/180)*pow(sin((longitude[i]-longitude[j])*pi/360),2)));
        }
    }
    return 1;
}




int main(){
    float matriks[15][15];
    int n = 0;
    char namaKota[15][255];
    char namaFile[255];
    scanf("%s", namaFile);

    if (open_init(namaFile, matriks, namaKota, &n) == 0){
        return 0;
    }
    printf("\n");
    for (int i = 0; i < n; i ++){
        for (int j = 0; j < n; j++){
            printf("%f ", matriks[i][j]);
        }
        printf("\n");
    }
}
