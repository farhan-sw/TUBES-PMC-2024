#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define N 4 // Number of cities

int graph[N][N] = {
    {0, 10, 15, 20},
    {10, 0, 35, 25},
    {15, 35, 0, 30},
    {20, 25, 30, 0}
};

int minCost = INT_MAX; // Initialize minimum cost to a large value

void branchAndBound(int path[], bool visited[], int currCost, int level) {
    if (level == N) {
        // If all cities have been visited, update the minimum cost
        if (graph[path[level - 1]][path[0]] != 0) {
            int totalCost = currCost + graph[path[level - 1]][path[0]];
            if (totalCost < minCost) {
                minCost = totalCost;
            }
        }
        return;
    }

    // Explore all unvisited cities
    for (int i = 0; i < N; i++) {
        if (!visited[i] && graph[path[level - 1]][i] != 0) {
            visited[i] = true;
            path[level] = i;
            branchAndBound(path, visited, currCost + graph[path[level - 1]][i], level + 1);
            visited[i] = false;
        }
    }
}

int main() {
    int path[N];
    bool visited[N];

    // Start from the first city
    path[0] = 0;
    visited[0] = true;

    // Initialize other cities as unvisited
    for (int i = 1; i < N; i++) {
        visited[i] = false;
    }

    // Call the Branch and Bound function
    branchAndBound(path, visited, 0, 1);

    // Print the minimum cost
    printf("Minimum cost for TSP: %d\n", minCost);

    //Print Path
    printf("Path: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", path[i]);
    }

    return 0;
}