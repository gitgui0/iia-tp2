
#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "utils.h"

//Devolve o custo
int trepaColinas(Solucao *atual, float distancias[MAX_C][MAX_C],int m, int nC, int* numIter,float pen, int *countValidos);
int recristalizacao(Solucao *atual, float distancias[MAX_C][MAX_C], int m, int nC, int* numIter,
                       float pen, int* countValidos, float temperatura, float resfriamento, float temperaturaFinal);
void geraSolucaoEvolutivo(Solucao *melhorGlobal, float distancias[MAX_C][MAX_C], int nC, int m, float pen,
                        Evolutivo ev, Solucao* pop, Solucao* pais, Solucao* filhos);


#endif
