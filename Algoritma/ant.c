#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "..\utils\openFile.h"

#define ALPHA 1         // Pheromone trail importance factor
#define BETA 2          // Heuristic information importance factor
#define RHO 0.5         // Pheromone evaporation rate
#define Q 100           // Pheromone deposit factor
#define MAX_ITER 1000   // Maximum number of iterations
#define NUM_ANTS 10     // Number of ants
#define MAX 255

int size;

void antColonyOptimization(float matriks[15][15], float **pheromones, float **deltaPheromones, int *bestTour, float *bestTourLength) {
    float *antDistances = (float*)malloc(NUM_ANTS * sizeof(float));
    float *pheromoneInfluence = (float*)malloc(size * sizeof(float));
    int *visited = (int*)malloc(size * sizeof(int));
    int *tour = (int*)malloc(size * sizeof(int));

    for (int iteration = 0; iteration < MAX_ITER; iteration++) {
        for (int ant = 0; ant < NUM_ANTS; ant++) {
            // Initialization
            memset(visited, 0, size * sizeof(int));
            tour[0] = rand() % size;
            visited[tour[0]] = 1;

            for (int step = 1; step < size; step++) {
                // Calculate pheromone influence
                float total = 0.0;
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        pheromoneInfluence[i] = pow(pheromones[tour[step - 1]][i], ALPHA) * pow(1.0 / matriks[tour[step - 1]][i], BETA);
                        total += pheromoneInfluence[i];
                    } else {
                        pheromoneInfluence[i] = 0.0;
                    }
                }

                // Roulette wheel selection
                float rnd = (float)rand() / RAND_MAX * total;
                float cumulative = 0.0;
                int next = -1;
                for (int i = 0; i < size; i++) {
                    if (!visited[i]) {
                        cumulative += pheromoneInfluence[i];
                        if (cumulative >= rnd) {
                            next = i;
                            break;
                        }
                    }
                }

                if (next == -1) {
                    // If no city was selected, choose a random one
                    for (int i = 0; i < size; i++) {
                        if (!visited[i]) {
                            next = i;
                            break;
                        }
                    }
                }

                tour[step] = next;
                visited[next] = 1;
            }

            // Calculate tour distance
            float distance = 0.0;
            for (int i = 0; i < size - 1; i++) {
                distance += matriks[tour[i]][tour[i + 1]];
            }
            distance += matriks[tour[size - 1]][tour[0]]; // Complete the tour

            antDistances[ant] = distance;

            // Update best tour
            if (distance < *bestTourLength || *bestTourLength == -1) {
                *bestTourLength = distance;
                memcpy(bestTour, tour, size * sizeof(int));
            }
        }

        // Update delta pheromones
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                deltaPheromones[i][j] = 0;
            }
        }
        for (int k = 0; k < NUM_ANTS; k++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    deltaPheromones[i][j] += Q / antDistances[k];
                }
            }
        }

        // Update pheromones
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                pheromones[i][j] = (1 - RHO) * pheromones[i][j] + deltaPheromones[i][j];
            }
        }
    }

    free(antDistances);
    free(pheromoneInfluence);
    free(visited);
    free(tour);
}

int ant(char path_file[MAX], char startCity[MAX]) {
    char namaFile[255];
    float matriks[15][15];
    int n = 0;
    char namaKota[15][255];

    open_init(path_file, matriks, namaKota, &n);

    size = n; // Update size to be the number of cities read

    // Initialize pheromones
    float **pheromones = (float**)malloc(size * sizeof(float*));
    float **deltaPheromones = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        pheromones[i] = (float*)malloc(size * sizeof(float));
        deltaPheromones[i] = (float*)malloc(size * sizeof(float));
        for (int j = 0; j < size; j++) {
            pheromones[i][j] = 1.0; // Initial pheromone level
            deltaPheromones[i][j] = 0.0;
        }
    }

    int *bestTour = (int*)malloc(size * sizeof(int));
    float bestTourLength = -1;

    // Find the index of the starting city
    int startingIndex;
    for (int i = 0; i < size; i++) {
        if (strcmp(startCity, namaKota[i]) == 0) {
            startingIndex = i;
            break;
        }
    }

    // Run Ant Colony Optimization
    srand(time(NULL)); // Initialize random seed
    antColonyOptimization(matriks, pheromones, deltaPheromones, bestTour, &bestTourLength);
    
    // Find the indices of the best tour
    int *bestTourIndices = (int*)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        bestTourIndices[i] = bestTour[i];
    }
    
    // Find the index of the starting city in the best tour
    int startIndexInTour = -1;
    for (int i = 0; i < size; i++) {
        if (bestTour[i] == startingIndex) {
            startIndexInTour = i;
            break;
        }
    }

    printf("Best route found:\n");
    for (int i = 0; i < size; i++) {
        printf("%s -> ", namaKota[bestTourIndices[(startIndexInTour + i) % size]]);
    }

    printf("%s\n", namaKota[bestTourIndices[startIndexInTour]]); // Print the starting city again to complete the tour

    printf("Best route distance: %.5lf km\n", bestTourLength);

    // Deallocate memory
    free(bestTourIndices);
    for (int i = 0; i < size; i++) {
        free(pheromones[i]);
        free(deltaPheromones[i]);
    }
    free(pheromones);
    free(deltaPheromones);
    return 0;
}