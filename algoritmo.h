
#ifndef ALGORITMO_H
#define ALGORITMO_H

#include "utils.h"

//Devolve o custo
int trepaColinas(Solucao *atual, float distancias[MAX_C][MAX_C],int m, int nC, int* numIter,float pen, int *countValidos, int tipoVizinhanca);
int recristalizacao(Solucao *atual, float distancias[MAX_C][MAX_C], int m, int nC, int* numIter,
                       float pen, int* countValidos, float temperaturaMaxima, float resfriamento, float temperaturaMinima, int tipoVizinhanca, int aceita);
void geraSolucaoEvolutivo(Solucao *melhorGlobal, float distancias[MAX_C][MAX_C], int nC, int m, float pen,
                        Evolutivo ev, Solucao* pop, Solucao* pais, Solucao* filhos);
void geraSolucaoHibrido(Solucao *melhorGlobal, float distancias[MAX_C][MAX_C], int nC, int m, float pen,
                        Evolutivo ev, Solucao* pop, Solucao* pais, Solucao* filhos, int abordagem, int algoritmoEscolhido, float temperaturaMaxima, float arrefecimento, float temperaturaMinima, int numIter, int tipoVizinhanca, int aceita);

#endif
