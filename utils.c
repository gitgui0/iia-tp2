#include "utils.h"

void printSol(Solucao* sol, int nC) {
    printf("Lista: ");
    for(int i=0; i<nC; i++) {
        printf("%d ", sol->sel[i]);
    }
    printf("\nNum selecionados: %d",sol->nSel);
}

void geraSolucaoInicial(Solucao* s, int m, int nC) {
    for (int i = 0; i < nC; i++) {
        s->sel[i] = 0;
    }
    s->nSel = 0;

    while (s->nSel < m) {
        // candidato random
        int r = rand() % nC;

        if (s->sel[r] == 0) {
            s->sel[r] = 1;
            s->nSel++;
        }
    }
}

void mostraMatrizDistancias(float distancias[MAX_C][MAX_C], int nC) {
    printf("\nMatriz Carregada:\n");
    for(int i=0; i<nC; i++) {
        for(int j=0; j<nC; j++) {
            printf("%.1f\t", distancias[i][j]);
        }
        printf("\n");
    }
}

