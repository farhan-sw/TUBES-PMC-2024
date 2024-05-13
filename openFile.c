#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#define MAX 255

// Masih dalam progress

int count_delim(char *string, char delim){
    int n = 0;
    for (int i = 0; i < strlen(string); i++){
        if (string[i] == delim){
            n += 1;
        }
    }
    return n;
}

int open_init(char *namaFile, int matriks[15][15], char namaKota[15][255], int *n){
    *n = 0;
    FILE *file = fopen(namaFile, "r");
    if (file == NULL){
        printf("\nFile tidak ada!");
        return 0;
    }
    
    char line[255];

    while (fgets(line, 255, file)){
        if (count_delim(line, ',') != 2){
            printf("\nTerdapat kesalahan format pada file!");
            return 0;
        }
        strcpy(namaKota[*n], strtok(line, ","));
        float latitude = atof(strtok(NULL, ","));
        float longitude = atof(strtok(NULL, "\n"));
        *n += 1;
    }
    return 1;
}




int main(){
    int matriks[15][15];
    int n = 0;
    char namaKota[15][255];
    char namaFile[255];
    scanf("%s", namaFile);

    if (open_init(namaFile, matriks, namaKota, &n) == 0){
        return 0;
    }
    printf("\n");
    for (int i = 0; i < n; i ++){
        printf("\n%s", namaKota[i]);
    }
}
