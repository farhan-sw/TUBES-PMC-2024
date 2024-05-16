#ifndef BNB_H
#define BNB_H

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

#define INF FLT_MAX

#include "..\utils\openFile.h"
#include "..\utils\constants.h"

#define MAX 255
#define MAX_CITY 15

void copyToFinal(int curr_path[]);
float firstMin(float adj[MAX_CITY][MAX_CITY], int i);
float secondMin(float adj[MAX_CITY][MAX_CITY], int i);
void TSPRec(float adj[MAX_CITY][MAX_CITY], float curr_bound, float curr_weight, int level, int curr_path[]);
void TSP(float adj[MAX_CITY][MAX_CITY], int start_city);

int bnb(char path_file[MAX_CHAR], char startCity[MAX_CHAR]);

#endif /* BRANCH_AND_BOUND_H */
