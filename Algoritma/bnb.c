#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

#include "..\utils\openFile.h"

#define MAX 255
#define MAX_CITY 15
#define INF INT_MAX

int N;
float minCost = FLT_MAX;
int final_path[MAX_CITY + 1];
bool visited[MAX_CITY];

// Struct untuk node pohon
typedef struct Node {
    float cost;
    int vertex;
    int level;
    int path[MAX_CITY];
    float reducedMatrix[MAX_CITY][MAX_CITY];
} Node;

// Fungsi untuk menginisialisasi node baru
Node* newNode(float parentMatrix[MAX_CITY][MAX_CITY], int path[MAX_CITY], int level, int i, int j) {
    Node* node = (Node*)malloc(sizeof(Node));
    memcpy(node->reducedMatrix, parentMatrix, sizeof(node->reducedMatrix));
    memcpy(node->path, path, sizeof(node->path));
    if (level != 0) node->path[level - 1] = i;
    node->level = level;
    node->vertex = j;
    return node;
}

// Fungsi untuk mengurangi baris matriks
void rowReduction(float reducedMatrix[MAX_CITY][MAX_CITY], float row[MAX_CITY]) {
    for (int i = 0; i < N; i++) row[i] = INF;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reducedMatrix[i][j] < row[i])
                row[i] = reducedMatrix[i][j];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reducedMatrix[i][j] != INF && row[i] != INF)
                reducedMatrix[i][j] -= row[i];
}

// Fungsi untuk mengurangi kolom matriks
void columnReduction(float reducedMatrix[MAX_CITY][MAX_CITY], float col[MAX_CITY]) {
    for (int i = 0; i < N; i++) col[i] = INF;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reducedMatrix[i][j] < col[j])
                col[j] = reducedMatrix[i][j];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (reducedMatrix[i][j] != INF && col[j] != INF)
                reducedMatrix[i][j] -= col[j];
}

// Fungsi untuk menghitung biaya dari matriks yang direduksi
float calculateCost(float reducedMatrix[MAX_CITY][MAX_CITY]) {
    float cost = 0;
    float row[MAX_CITY];
    float col[MAX_CITY];
    rowReduction(reducedMatrix, row);
    columnReduction(reducedMatrix, col);
    for (int i = 0; i < N; i++) {
        cost += (row[i] != INF) ? row[i] : 0;
        cost += (col[i] != INF) ? col[i] : 0;
    }
    return cost;
}

// Fungsi untuk mencetak jalur terbaik
void printPath(int path[MAX_CITY]) {
    for (int i = 0; i < N; i++) printf("%d -> ", path[i]);
    printf("0\n");
}

// Fungsi untuk memecahkan TSP menggunakan Branch and Bound
void solveTSP(float costMatrix[MAX_CITY][MAX_CITY]) {
    Node* root = newNode(costMatrix, final_path, 0, -1, 0);
    root->cost = calculateCost(root->reducedMatrix);
    root->path[0] = 0;
    
    // Priority queue untuk node hidup
    Node* pq[MAX * MAX];
    int pqSize = 0;

    pq[pqSize++] = root;

    while (pqSize > 0) {
        Node* min = pq[0];
        for (int i = 1; i < pqSize; i++)
            if (pq[i]->cost < min->cost) min = pq[i];

        for (int i = 0; i < pqSize; i++)
            if (pq[i] == min) {
                pq[i] = pq[--pqSize];
                break;
            }

        int i = min->vertex;

        if (min->level == N - 1) {
            min->path[min->level] = min->vertex;
            min->path[min->level + 1] = 0;
            float curr_cost = min->cost;
            if (curr_cost < minCost) {
                minCost = curr_cost;
                memcpy(final_path, min->path, sizeof(final_path));
            }
            free(min);
            continue;
        }

        for (int j = 0; j < N; j++) {
            if (min->reducedMatrix[i][j] != INF && !visited[j]) {
                Node* child = newNode(min->reducedMatrix, min->path, min->level + 1, i, j);
                child->cost = min->cost + min->reducedMatrix[i][j] + calculateCost(child->reducedMatrix);
                pq[pqSize++] = child;
            }
        }

        visited[min->vertex] = true;
        free(min);
    }
}

int findCityIndex(char cityName[MAX_CITY][MAX], char startCity[MAX]) {
    for (int i = 0; i < N; i++)
        if (strcmp(cityName[i], startCity) == 0) return i;
    return -1;
}

int bnb(char path_file[MAX], char startCity[MAX]) {
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    char cityName[MAX_CITY][MAX];
    int numVertices;

    open_init(path_file, adjacencyMatrix, cityName, &numVertices);
    N = numVertices;
    
    memset(final_path, -1, sizeof(final_path));
    memset(visited, false, sizeof(visited));

    int startCityInd = findCityIndex(cityName, startCity);
    if (startCityInd == -1) {
        printf("Start city not found!\n");
        return -1;
    }

    solveTSP(adjacencyMatrix);

    printf("Best route distance: %.5f km\n", minCost);
    printf("Best route: ");
    printPath(final_path);

    return 0;
}
