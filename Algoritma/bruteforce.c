/*
    Author      : Ahmad Ramadhan
    NIM         : 13222110
    Description : Finding best route for travelling salesman problem
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "..\utils\openFile.h"
#include <time.h>
#include <assert.h>

int vis[15], permutation[15], n;
int best_route[15];
float matriks[15][15];
char namaKota[15][255];
char namaFile[255];
double min_cost = 1e9;

void generate(int x) {
    if(x >= n) { // basis rekursi
        double cost = 0;
        for(int i = 1; i < n; ++i) {
            cost += matriks[permutation[i - 1]][permutation[i]];
        }

        cost += matriks[permutation[n - 1]][permutation[0]];

        if(cost < min_cost) { // update minimum cost and the best route
            min_cost = cost;
            for(int i = 0; i < n; ++i) {
                best_route[i] = permutation[i];
            }
        }
    } else {
        for(int i = 0; i < n; ++i) {
            if(vis[i]) continue;
            vis[i] = 1;
            permutation[x] = i;
            generate(x + 1);
            vis[i] = 0;
        }
    }
}

void output_best_route(){
    puts("Best route found:");
    for(int i = 0; i < n; ++i) {
        printf("%s -> ", namaKota[best_route[i]]);
    }

    printf("%s\n", namaKota[best_route[0]]);
    printf("Best route distance: %.10lf\n",min_cost);
}

void find_starting_point(char *point, int *index) {
    for(int i = 0; i < n; ++i) {
        if(strcmp(point, namaKota[i]) == 0) {
            *index = i;
            return;
        }
    }
}

void bruteForce(char namaFile[255], char *startingPoint) {
    int index = -1;
    find_starting_point(startingPoint, &index); // finding starting point with linier search
    assert(index != -1);        // make sure index nya nemu
    vis[index] = 1;             // node starting point ditandai
    permutation[0] = index;     // mengisi permutasi pertama dengan starting point
    generate(1);                // mengenerate permutasi kota-kota yang akan dituju
    output_best_route();        // display minimum cost ans best route 
}