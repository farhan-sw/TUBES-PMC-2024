#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "..\utils\openFile.h"

#define MAX 255
#define SWARM_SIZE 200
#define MAX_ITERATIONS 100
#define WEIGHT 0.5
#define C1 1.0
#define C2 1.0
#define INF_PSO 99999999999999999


typedef struct {
    int path[MAX];
    double fitness;
    int pbest[MAX];
    double pbest_fitness;
    double velocity[MAX];
} Particle;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

double calculate(float adjacencyMatrix[15][15], int path[], int numVertices) {
    double total = 0.0;
    for (int i = 0; i < numVertices - 1; i++) {
        total += adjacencyMatrix[path[i]][path[i + 1]];
    }
    total += adjacencyMatrix[path[numVertices - 1]][path[0]]; 
    return total;
}

void initialize_pso(Particle particles[], int numVertices, int swarmSize, int startCityIndex) {
    int tempPath[MAX];
    for (int i = 0; i < numVertices; i++) {
        tempPath[i] = i;
    }

    if (startCityIndex != 0) {
        swap(&tempPath[0], &tempPath[startCityIndex]);
    }

    for (int i = 0; i < swarmSize; i++) {
        memcpy(particles[i].path, tempPath, sizeof(int) * numVertices);
        for (int j = 1; j < numVertices; j++) { 
            int rt = rand() % (numVertices - 1) + 1; 
            int temp = particles[i].path[j];
            particles[i].path[j] = particles[i].path[rt];
            particles[i].path[rt] = temp;
        }
        particles[i].fitness = INF_PSO;
        particles[i].pbest_fitness = INF_PSO;
        memset(particles[i].velocity, 0, sizeof(double) * numVertices);
    }
}
void update_particles(Particle particles[], int numVertices, int swarmSize, int gbest[], double inertia, double cognitive, double social) {
    for (int i = 0; i < swarmSize; i++) {
        for (int j = 0; j < numVertices; j++) {
            double r1 = (double)rand() / RAND_MAX;
            double r2 = (double)rand() / RAND_MAX;
            particles[i].velocity[j] = inertia * particles[i].velocity[j] + cognitive * r1 * (particles[i].pbest[j] - particles[i].path[j]) + social * r2 * (gbest[j] - particles[i].path[j]);
            
            if (particles[i].velocity[j] > 0.5) particles[i].velocity[j] = 0.5;
            if (particles[i].velocity[j] < -0.5) particles[i].velocity[j] = -0.5;
        }
        for (int j = 0; j < numVertices; j++) {
            int swapIndex = (j + (int)particles[i].velocity[j]) % numVertices;
            if (swapIndex < 0) swapIndex += numVertices;
            swap(&particles[i].path[j], &particles[i].path[swapIndex]);
        }
    }
}

void copy(int src[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = src[i];
    }
}

int particles(char path_file[MAX], char startCity[MAX]) {
    float adjacencyMatrix[15][15];
    int numVertices;        
    char cityName[15][MAX];

    open_init(path_file, adjacencyMatrix, cityName, &numVertices);

    int destinationIndex = -1;
    for (int i = 0; i < numVertices; i++) {
        if (strcmp(cityName[i], startCity) == 0) {
            destinationIndex = i;
            break;
        }
    }

    Particle particles[SWARM_SIZE];
    int gbest[MAX];
    double gbest_fitness = INF_PSO;

    initialize_pso(particles, numVertices, SWARM_SIZE, destinationIndex);

    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        for (int i = 0; i < SWARM_SIZE; i++) {
            particles[i].fitness = calculate(adjacencyMatrix, particles[i].path, numVertices);

            if (particles[i].fitness < particles[i].pbest_fitness) {
                particles[i].pbest_fitness = particles[i].fitness;
                copy(particles[i].path, particles[i].pbest, numVertices);
            }

            if (particles[i].fitness < gbest_fitness) {
                gbest_fitness = particles[i].fitness;
                copy(particles[i].path, gbest, numVertices);
            }
        }
        update_particles(particles, numVertices, SWARM_SIZE, gbest, WEIGHT, C1, C2);
    }
    printf("Best route found:\n");
    for (int i = 0; i < numVertices; i++) {
        printf("%s -> ", cityName[gbest[i]]);
    }
    printf("%s\n", cityName[gbest[0]]);
    printf("Best route distance: %.5f km\n", gbest_fitness);
    return 0;
}
