#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

#define INF FLT_MAX

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

void printPath(int* path, int level) {
    for (int i = 0; i <= level; i++) {
        printf("%d ", path[i]);
    }
    printf("0\n");
}

float solve(int startCity, int N, float costMatrix[N][N]) {
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
            printPath(min->path, min->level);
            float result = min->cost;
            for (int k = 0; k < N; k++) {
                free(min->reducedMatrix[k]);
            }
            free(min->reducedMatrix);
            free(min->path);
            free(min);
            return result;
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
    return 0;
}

int main() {
    int N = 4; // Number of cities
    float costMatrix[4][4] = {
        {INF, 10.0f, 15.0f, 20.0f},
        {10.0f, INF, 35.0f, 25.0f},
        {15.0f, 35.0f, INF, 30.0f},
        {20.0f, 25.0f, 30.0f, INF}
    };

    int startCity;
    printf("Enter the starting city (0 to %d): ", N-1);
    scanf("%d", &startCity);

    // Print adjacency matrix
    printf("Adjacency matrix:\n");

    clock_t start = clock();
    float cost = solve(startCity, N, costMatrix);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Cost: %.2f\n", cost);
    printf("Time: %.7f seconds\n", time_taken);

    return 0;
}
