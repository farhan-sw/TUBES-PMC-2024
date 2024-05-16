#ifndef ANT_H
#define ANT_H

#include <math.h>
#include <time.h>


#define ALPHA 1         // Pheromone trail importance factor
#define BETA 2          // Heuristic information importance factor
#define RHO 0.5         // Pheromone evaporation rate
#define Q 100           // Pheromone deposit factor
#define MAX_ITER 1000   // Maximum number of iterations
#define NUM_ANTS 10     // Number of ants
#define MAX 255

void antColonyOptimization(float matriks[15][15], float **pheromones, float **deltaPheromones, int *bestTour, float *bestTourLength);

int ant(char path_file[MAX], char startCity[MAX]);

#endif /* ANT_H */
