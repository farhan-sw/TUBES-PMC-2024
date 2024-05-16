#include <iostream>
#include <limits.h>
#include <cstring>
using namespace std;

const int N = 4;
int final_path[N + 1];
int final_res = INT_MAX; // Declare final_res here as a global variable
bool visited[N];

void copyToFinal(int curr_path[]) {
    for (int i = 0; i < N; i++)
        final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
}

int firstMin(int adj[N][N], int i) {
    int min = INT_MAX;
    for (int k = 0; k < N; k++)
        if (adj[i][k] < min && i != k)
            min = adj[i][k];
    return min;
}

int secondMin(int adj[N][N], int i) {
    int first = INT_MAX, second = INT_MAX;
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

void TSPRec(int adj[N][N], int curr_bound, int curr_weight,
            int level, int curr_path[]) {
    if (level == N) {
        if (adj[curr_path[level - 1]][curr_path[0]] != 0) {
            int curr_res = curr_weight +
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
            visited[i] == false) {
            int temp = curr_bound;
            curr_weight += adj[curr_path[level - 1]][i];
            if (level == 1)
                curr_bound -= ((firstMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);
            else
                curr_bound -= ((secondMin(adj, curr_path[level - 1]) +
                                firstMin(adj, i)) / 2);
            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = true;
                TSPRec(adj, curr_bound, curr_weight, level + 1,
                        curr_path);
            }
            curr_weight -= adj[curr_path[level - 1]][i];
            curr_bound = temp;
            memset(visited, false, sizeof(visited));
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

void TSP(int adj[N][N], int start_city) {
    int curr_path[N + 1];
    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    memset(visited, 0, sizeof(curr_path));
    for (int i = 0; i < N; i++)
        curr_bound += (firstMin(adj, i) + secondMin(adj, i));
    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 :
                                    curr_bound / 2;
    visited[start_city] = true;
    curr_path[0] = start_city;
    TSPRec(adj, curr_bound, 0, 1, curr_path);
}

int main() {
    int adj[N][N] = { {0, 10, 15, 20},
                      {10, 0, 35, 25},
                      {15, 35, 0, 30},
                      {20, 25, 30, 0}
                    };
    int start_city;
    cout << "Enter the index of the starting city (0 to " << N-1 << "): ";
    cin >> start_city;
    if (start_city < 0 || start_city >= N) {
        cout << "Invalid starting city index!";
        return 1;
    }
    TSP(adj, start_city);

    // Calculate Minimum Cost from final_path and costMatrix
    float minCost = 0;
    for (int i = 0; i < N; i++) {
        printf("Cost from %d to %d: %d\n", final_path[i], final_path[i + 1], adj[final_path[i]][final_path[i + 1]]);
        minCost += adj[final_path[i]][final_path[i + 1]];
    }
    cout << "Minimum Cost: " << minCost << endl;

    printf("Path Taken : ");
    for (int i = 0; i <= N; i++)
        printf("%d ", final_path[i]);
    return 0;
}
