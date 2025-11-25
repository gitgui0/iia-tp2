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