

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_C 500



typedef struct {
    int sel[MAX_C]; // 1-selcionado, 0-nao selecionado
    float media; // media com a funcao
    int nSel; // numero de selecionados ( '1' )
} Solucao;

void printSol(Solucao* sol, int nC);

void geraSolucaoInicial(Solucao* s, int m, int nC);

void mostraMatrizDistancias(float distancias[MAX_C][MAX_C], int nC);




#endif //UTILS_H