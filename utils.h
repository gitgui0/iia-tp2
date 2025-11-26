

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

int leInfo(int* nC, int* m, float distancias[MAX_C][MAX_C]);

int geraNumEntre(int inf, int sup);

void copiaSolucao(Solucao* destino, Solucao* origem, int nC);

float calculaMaxDistancia(float distancias[MAX_C][MAX_C], int nC);

float calculaValorPenalizacao(int nSel, int m, float pen);
void mostraRelatorioPenalizacao(float penAplicada, float fitnessBase);

#endif //UTILS_H