#include "funcao.h"

float devolveMedia(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m) {
    float sum=0;
    float div = (float)m;
    for(int i=0; i<nC; i++) {
        if (sol->sel[i] == 1) {
            for (int j = i + 1; j < nC; j++) {
                if (sol->sel[j] == 1) {
                    float val = distancias[i][j];
                    sum += val;
                }
            }
        }
    }
    return sum / div;
}

float calculaPenalidade(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m,float pen) {
    float distMedia;

    //valor base (Distância Média)
    distMedia = devolveMedia(sol, distancias, nC, m);

    // se m for diferen de numeor de soluçoes "froça" a mudança para ser igual
    if (sol->nSel != m) {
        int diferenca = abs(sol->nSel - m);
        return distMedia - diferenca * pen;
    }
    return distMedia;
}


void avaliaPopulacao(Solucao *pop, float distancias[MAX_C][MAX_C], int nC, int m,float pen) {
    int i;

    for (i = 0; i < nC; i++) {
        pop[i].media = calculaPenalidade(&pop[i], distancias, nC, m,pen);
    }
}