
#ifndef PROC_H
#define PROC_H


int lerParametrosRecristalizacao(float* temperaturaMaxima, float* arrefecimento,float* temperaturaMinima, int *vizinhanca, int* aceita);
int lerParametrosEvolutivo(Evolutivo *ev);
int lerParametrosHibrido(Evolutivo *ev, int *algoritmo,int* abordagem, float* temperaturaMaxima, float* arrefecimento,float* temperaturaMinima, int *numIterLocal, int *aceita, int* vizinhanca);


#endif
