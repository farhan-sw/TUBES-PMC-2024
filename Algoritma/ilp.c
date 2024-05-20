#include <stdio.h>
#include <stdlib.h>
#include <glpk.h> // Library untuk melakukan ILP
#include <math.h>
#include "..\utils\openFile.h"
#include <time.h>

#define MAX 255
#define INF 999999

// SUPPORTING FUNCTION
// Mencari indeks kota yang dicari
int find_city_index(char listOfCities[15][MAX], char city[MAX]) {
    for (int j = 0; j < 15; j++) {
        if (strcmp(city, listOfCities[j]) == 0) {
            return j + 1;
        }
    }
    // Apabila tidak tersedia
    return -1;
}
// MAIN FUNCTION
// Fungsi Integer Linear Programming
int ilp(char path_file[MAX], char startCity[MAX]) {
    char file[MAX];
    float adjacencyMatrix[15][15];
    int numVertices;        
    char cityName[15][MAX];

    // Membuka file dan menyimpan jarak dalam graph
    open_init(path_file, adjacencyMatrix, cityName, &numVertices);

    // Membuat objek problem GPLK
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "ShortestPath");

    // Fungsi 
    int num_vars = numVertices;
    glp_add_cols(lp, num_vars);
    for (int i = 1; i <= num_vars; i++) {
        char cname[10];
        sprintf(cname, "x%d", i);
        glp_set_col_name(lp, i, cname);
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 1.0); // Nilai biner untuk menunjukkan kota sudah dikunjungi atau belum
        glp_set_col_kind(lp, i, GLP_BV); // Variabel biner
        glp_set_obj_coef(lp, i, 0.0); // Kota yang belum dikunjungi (koef 0) akan dikunjungi
    }

    // Menambah batas-batasan
    glp_add_rows(lp, numVertices);
    for (int i = 1; i <= numVertices; i++) {
        char rname[10];
        sprintf(rname, "c%d", i);
        glp_set_row_name(lp, i, rname);
        glp_set_row_bnds(lp, i, GLP_FX, 1.0, 1.0); // Setiap kota hanya dikunjungi sekali saja (dalam satu rute)
    }

    // Membuat matriks sesuai dengan batasan
    int constr_count = 1;
    for (int i = 1; i <= numVertices; i++) {
        for (int j = 1; j <= numVertices; j++) {
            if (i != j) {
                int coef = adjacencyMatrix[i - 1][j - 1];
                if (coef != INF) {
                    int ia[2] = {i, j};
                    int ja[2] = {find_city_index(cityName, cityName[i - 1]), find_city_index(cityName, cityName[j - 1])};
                    double ar[2] = {1.0, -1.0};
                    glp_set_mat_row(lp, constr_count, 1, ia, ar);
                    constr_count++;
                }
            }
        }
    }

    // Set optimalisasi menjadi minimal
    glp_set_obj_dir(lp, GLP_MIN);

    // Selesaikan ILP
    glp_iocp iocp;
    glp_init_iocp(&iocp);
    iocp.presolve = GLP_ON;
    glp_intopt(lp, &iocp);

    // Print Solusi
    double total_distance = 0.0;
    int current = find_city_index(cityName, startCity);
    printf("Best route found:\n%s", startCity);
    for (int step = 0; step < numVertices; step++) {
        for (int i = 1; i <= num_vars; i++) {
            if (glp_mip_col_val(lp, i) == 1 && find_city_index(cityName, cityName[i - 1]) != current) {
                int next = find_city_index(cityName, cityName[i - 1]) - 1;
                printf(" -> %s", cityName[next]);
                total_distance += adjacencyMatrix[currentCity - 1][next];
                current = next + 1;
                break;
            }
        }
    }
    printf("Total distance: %.2f\n", total_distance);

    // Free memory
    glp_delete_prob(lp);

    return 0;
}
