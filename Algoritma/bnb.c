#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

#define INF FLT_MAX

#include "..\utils\openFile.h"
#include "..\utils\constants.h"

int isDebug = 0;

// Maksimal nilai N yang dapat diterima
#define MAX_N 15

int final_path[MAX_N + 1];
float final_res = FLT_MAX;
int visited[MAX_N];
int N;

void copyToFinal(int curr_path[]) {
    for (int i = 0; i < N; i++)
        final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
}

float firstMin(float adj[MAX_N][MAX_N], int i) {
    float min = FLT_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

float secondMin(float adj[MAX_N][MAX_N], int i) {
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

void TSPRec(float adj[MAX_N][MAX_N], float curr_bound, float curr_weight,
            int level, int curr_path[]) {
    if (level == N) {
        if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
            float curr_res = curr_weight +
                adj[curr_path[level - 1]][curr_path[0]];
            if (curr_res < final_res) {
                copyToFinal(curr_path);
                final_res = curr_res;
            }
        }
        return;
    }
    for (int i = 0; i < N; i++) {
        if (adj[curr_path[level - 1]][i] != 0 &&
            visited[i] == 0) {
            float temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];
            if (level == 1)
                curr_bound -= ((firstMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);
            else
                curr_bound -= ((secondMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);
            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = 1;
                TSPRec(adj, curr_bound, curr_weight, level + 1,
                        curr_path);
            }
            curr_weight -= adj[curr_path[level - 1]][i];
            curr_bound = temp;
            memset(visited, 0, sizeof(visited));
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = 1;
        }
    }
}

void TSP(float adj[MAX_N][MAX_N], int start_city) {
    int curr_path[MAX_N + 1];
    float curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(visited));
    for (int i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) + secondMin(adj, i));
    curr_bound = (curr_bound < 1) ? 1 :
                                    curr_bound / 2;
    visited[start_city] = 1;
    curr_path[0] = start_city;
    TSPRec(adj, curr_bound, 0, 1, curr_path);
}

int bnb(char path_file[MAX_CHAR], char startCity[MAX_CHAR]) {
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    char cityName[MAX_CITY][MAX_CHAR];

    open_init(path_file, adjacencyMatrix, cityName, &N);

    // Cari index kota awal
    int startCityIndex = isCityExist(cityName, startCity, N);

    // Copy Adjacency Matrix ke adj matrix berukuran numVertices
    float adj[MAX_CITY][MAX_CITY];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            adj[i][j] = adjacencyMatrix[i][j];
        }
    }

    if(isDebug){
    printf("N: %d\n", N);
    printf("Start City Index: %d\n", startCityIndex);}

    // Algoritma BNB
    TSP(adj, startCityIndex);

    // printf("Path Taken : ");
    // for (int i = 0; i <= N; i++)
    //     printf("%d ", final_path[i]);

    // Print Rute
    printf("Best route found: \n");
    for (int i = 0; i <= N; i++) {
        printf("%s", cityName[final_path[i]]);
        if (i != N) {
            printf(" -> ");
        }
    }
    printf("\n");

    // Calculate Minimum Cost from final_path and costMatrix
    float minCost = 0;
    for (int i = 0; i < N; i++) {
        if(isDebug)
            printf("Cost from %d to %d: %.2f\n", final_path[i], final_path[i + 1], adj[final_path[i]][final_path[i + 1]]);
        minCost += adj[final_path[i]][final_path[i + 1]];
    }
    printf("Best route distance: %.5f km\n", minCost);
    

    return 0;
}
