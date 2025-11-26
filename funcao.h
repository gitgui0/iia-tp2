
#ifndef FUNCAO_H
#define FUNCAO_H

#include "utils.h"

float devolveMedia(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m);
float calculaPenalidade(Solucao* sol, float distancias[MAX_C][MAX_C], int nC, int m,float pen);
void avaliaPopulacao(Solucao *pop, float distancias[MAX_C][MAX_C], int nC, int m,float pen);

#endif //FUNCAO_H
