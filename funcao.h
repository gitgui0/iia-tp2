
#ifndef FUNCAO_H
#define FUNCAO_H

#include "utils.h"

float devolveMedia(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m);
float calculaPenalidade(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m,float pen);
void avaliaPopulacao(Solucao *pop, int popsize, float distancias[MAX_C][MAX_C], int nC, int m, int tipoReparacao);

#endif //FUNCAO_H
