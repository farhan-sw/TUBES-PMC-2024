#ifndef PSO_H
#define PSO_H

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
    int partikel_best[MAX];
    double partikel_best_fitness;
    double velocity[MAX];
} Particle;

// Function declarations
double calculate(float adjacencyMatrix[15][15], int path[], int numVertices);
void initialize_pso(Particle particles[], int numVertices, int swarmSize, int destinationIndex);
void swap(int *a, int *b);
void update_particles(Particle particles[], int numVertices, int swarmSize, int global_best[], double inertia, double cognitive, double social);
void copy(int src[], int copied[], int size);
int particles(char path_file[MAX], char startCity[MAX]);

#endif // PSO_H
