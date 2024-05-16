#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define N 6

int costMatrix[N][N] = {
    {0, 10, 15, 20, 25, 30},
    {10, 0, 35, 40, 45, 50},
    {15, 35, 0, 55, 60, 65},
    {20, 40, 55, 0, 70, 75},
    {25, 45, 60, 70, 0, 80},
    {30, 50, 65, 75, 80, 0}
};

int path[N];
bool visited[N];

int minCost = INT_MAX;

void tspBranchAndBound(int currCity, int currCost, int level) {
    if (level == N - 1) {
        if (costMatrix[currCity][0] != 0) {
            int totalCost = currCost + costMatrix[currCity][0];
            if (totalCost < minCost) {
                minCost = totalCost;
                for (int i = 0; i < N; i++) {
                    path[i] = visited[i];
                }
            }
        }
        return;
    }

    for (int nextCity = 0; nextCity < N; nextCity++) {
        if (!visited[nextCity] && costMatrix[currCity][nextCity] != 0) {
            visited[nextCity] = true;
            tspBranchAndBound(nextCity, currCost + costMatrix[currCity][nextCity], level + 1);
            visited[nextCity] = false;
        }
    }
}

int main() {
    for (int i = 0; i < N; i++) {
        visited[i] = false;
    }
    visited[0] = true;

    tspBranchAndBound(0, 0, 0);

    printf("Minimum cost: %d\n", minCost);
    printf("Path: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", path[i]);
    }
    printf("0\n");

    return 0;
}