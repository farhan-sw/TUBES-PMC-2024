#ifndef BF_H
#define BF_H

#include <stdbool.h>
#define MAX 255

void generate(int x);

void output_best_route();

void find_starting_point();

void bruteForce(char namaFile[MAX], char startingPoint[MAX]); 

#endif

