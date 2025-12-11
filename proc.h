
#ifndef PROC_H
#define PROC_H


int lerParametrosRecristalizacao(float* temperaturaInicial, float* arrefecimento,float* temperaturaFinal, int *vizinhanca, int* aceita);
int lerParametrosEvolutivo(Evolutivo *ev);
int lerParametrosHibrido(Evolutivo *ev, int *algoritmo,int* abordagem, float* temperaturaInicial, float* arrefecimento,float* temperaturaFinal, int *numIterLocal, int *aceita);


#endif
