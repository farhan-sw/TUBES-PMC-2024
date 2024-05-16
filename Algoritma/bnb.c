#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

#define INF FLT_MAX

#include "..\utils\openFile.h"
#include "..\utils\constants.h"

// Node structure for State Space Tree
typedef struct Node {
    float** reducedMatrix;
    float cost;
    int vertex;
    int level;
    int* path;
} Node;

// Function to allocate a new node
Node* newNode(float** parentMatrix, int level, int i, int j, int* path, int N) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->reducedMatrix = (float**)malloc(N * sizeof(float*));
    for (int k = 0; k < N; k++) {
        node->reducedMatrix[k] = (float*)malloc(N * sizeof(float));
        for (int l = 0; l < N; l++) {
            node->reducedMatrix[k][l] = parentMatrix[k][l];
        }
    }
    for (int k = 0; level != 0 && k < N; k++) {
        node->reducedMatrix[i][k] = INF;
        node->reducedMatrix[k][j] = INF;
    }
    node->reducedMatrix[j][0] = INF;
    node->level = level;
    node->vertex = j;
    node->path = (int*)malloc((level + 1) * sizeof(int));
    for (int k = 0; k < level; k++) {
        node->path[k] = path[k];
    }
    node->path[level] = j;
    return node;
}

void rowReduction(float** reducedMatrix, float* row, int N) {
    for (int i = 0; i < N; i++) {
        row[i] = INF;
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] < row[i]) {
                row[i] = reducedMatrix[i][j];
            }
        }
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[i][j] != INF && row[i] != INF) {
                reducedMatrix[i][j] -= row[i];
            }
        }
    }
}

void columnReduction(float** reducedMatrix, float* col, int N) {
    for (int i = 0; i < N; i++) {
        col[i] = INF;
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[j][i] < col[i]) {
                col[i] = reducedMatrix[j][i];
            }
        }
        for (int j = 0; j < N; j++) {
            if (reducedMatrix[j][i] != INF && col[i] != INF) {
                reducedMatrix[j][i] -= col[i];
            }
        }
    }
}

float calculateCost(float** reducedMatrix, int N) {
    float cost = 0;
    float* row = (float*)malloc(N * sizeof(float));
    rowReduction(reducedMatrix, row, N);
    float* col = (float*)malloc(N * sizeof(float));
    columnReduction(reducedMatrix, col, N);
    for (int i = 0; i < N; i++) {
        if (row[i] != INF) {
            cost += row[i];
        }
        if (col[i] != INF) {
            cost += col[i];
        }
    }
    free(row);
    free(col);
    return cost;
}

int compareNodes(const void* a, const void* b) {
    return ((Node*)a)->cost - ((Node*)b)->cost;
}

Node* solve(int startCity, int N, float costMatrix[N][N]) {
    float** initialMatrix = (float**)malloc(N * sizeof(float*));
    for (int i = 0; i < N; i++) {
        initialMatrix[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            initialMatrix[i][j] = costMatrix[i][j];
        }
    }

    int* initialPath = (int*)malloc(sizeof(int));
    initialPath[0] = startCity;
    Node* root = newNode(initialMatrix, 0, -1, startCity, initialPath, N);
    root->cost = calculateCost(root->reducedMatrix, N);
    free(initialPath);

    Node* pq[N * N];
    int pqSize = 0;
    pq[pqSize++] = root;

    while (pqSize > 0) {
        qsort(pq, pqSize, sizeof(Node*), compareNodes);
        Node* min = pq[--pqSize];
        int i = min->vertex;

        if (min->level == N - 1) {
            min->path[min->level + 1] = startCity;
            for (int k = 0; k < N; k++) {
                free(min->reducedMatrix[k]);
            }
            free(min->reducedMatrix);
            return min;
        }

        for (int j = 0; j < N; j++) {
            if (min->reducedMatrix[i][j] != INF) {
                Node* child = newNode(min->reducedMatrix, min->level + 1, i, j, min->path, N);
                child->cost = min->cost + min->reducedMatrix[i][j] + calculateCost(child->reducedMatrix, N);
                pq[pqSize++] = child;
            }
        }
        for (int k = 0; k < N; k++) {
            free(min->reducedMatrix[k]);
        }
        free(min->reducedMatrix);
        free(min->path);
        free(min);
    }
    return NULL;
}

int bnb(char path_file[MAX_CHAR], char startCity[MAX_CHAR]) {
    float adjacencyMatrix[MAX_CITY][MAX_CITY];
    char cityName[MAX_CITY][MAX_CHAR];
    int numVertices;

    open_init(path_file, adjacencyMatrix, cityName, &numVertices);
    // Pastikan kota dari kota itu sendiri INF
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i][i] = INF;
    }

    // Cari index kota awal
    int startCityIndex = isCityExist(cityName, startCity, numVertices);

    // Algoritma BNB
    Node* finalNode = solve(startCityIndex, numVertices, adjacencyMatrix);

    if (finalNode != NULL) {
        printf("Best route found: \n");
        for (int i = 0; i < numVertices; i++) {
            printf("%s -> ", cityName[finalNode->path[i]]);
        }
        printf("%s\n", cityName[finalNode->path[0]]);

        /// Calculate Cost from Path
        float cost = 0.0;
        // Start calculating cost from the second city to the last city and back to the start city
        for (int i = 1; i < numVertices; i++) {
            cost += adjacencyMatrix[finalNode->path[i - 1]][finalNode->path[i]];
        }

        printf("Best route distance: %.5f km\n", cost);

        free(finalNode->path);
        free(finalNode);
    }

    return 0;
}
