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

void reparaSolucaoAleatoria(Solucao *s, int m, int nC) {
    s->nSel = 0;
    for(int i=0; i<nC; i++) {
        if(s->sel[i]) s->nSel++;
    } //Precisa de recalcular nSel

    while (s->nSel > m) { // Remove excesso
        int r = geraNumEntre(0, nC - 1);
        if (s->sel[r] == 1) {
            s->sel[r] = 0;
            s->nSel--;
        }
    }
    while (s->nSel < m) { // Adiciona falta
        int r = geraNumEntre(0, nC - 1);
        if (s->sel[r] == 0) {
            s->sel[r] = 1;
            s->nSel++;
        }
    }
}



//------Evolutivo
void reparaSolucaoHeuristica(Solucao *s, int m, int nC, float distancias[MAX_C][MAX_C]) {
    s->nSel = 0;
    for(int i=0; i<nC; i++) if(s->sel[i]) s->nSel++;

    // Remove quem está "perto demais" dos outros (contribui pouco)
    while (s->nSel > m) {
        int pior_idx = -1;
        float menor_dist_total = 99999999.0;

        for (int i = 0; i < nC; i++) {
            if (s->sel[i] == 1) {
                float dist_sum = 0;
                // Calcula a distancia de s->sel[i] de todos
                for (int j = 0; j < nC; j++) {
                    if (i != j && s->sel[j] == 1) dist_sum += distancias[i][j];
                }

                if (dist_sum < menor_dist_total) {
                    menor_dist_total = dist_sum;
                    pior_idx = i;
                }
            }
        }
        if (pior_idx != -1) {
            s->sel[pior_idx] = 0;
            s->nSel--;
        }
        else break; // Segurança
    }

    // Adiciona quem está "mais longe" dos selecionados
    while (s->nSel < m) {
        int melhor_idx = -1;
        float maior_dist_total = -1.0;

        for (int i = 0; i < nC; i++) {
            if (s->sel[i] == 0) {
                float dist_sum = 0;
                // Calcula a distancia de s->sel[i] de todos
                for (int j = 0; j < nC; j++) {
                    if (s->sel[j] == 1) dist_sum += distancias[i][j];
                }
                if (dist_sum > maior_dist_total) {
                    maior_dist_total = dist_sum;
                    melhor_idx = i;
                }
            }
        }
        if (melhor_idx != -1) {
            s->sel[melhor_idx] = 1;
            s->nSel++;
        }
        else break;
    }
}

void avaliaPopulacao(Solucao *pop, int popsize, float distancias[MAX_C][MAX_C], int nC, int m, int tipoReparacao) {
    for (int i = 0; i < popsize; i++) {

        if (tipoReparacao == 1) {
            reparaSolucaoAleatoria(&pop[i], m, nC);
        }
        else if (tipoReparacao == 2) {
            reparaSolucaoHeuristica(&pop[i], m, nC, distancias);
        }


        if (tipoReparacao == 0) {
            pop[i].nSel = 0;
            for(int k=0; k<nC; k++) if(pop[i].sel[k]) pop[i].nSel++;

            pop[i].media = calculaPenalidade(&pop[i], distancias, nC, m, 9999.0);
        }
        else {
            pop[i].media = devolveMedia(&pop[i], distancias, nC, m);
        }

    }
}
