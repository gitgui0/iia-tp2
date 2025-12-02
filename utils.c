#include "utils.h"
#include <string.h>

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


//0->certo, 1->erro
int leInfo(int* nC, int* m, float distancias[MAX_C][MAX_C]) {
    char fich[100];
    char escolha[100];

    printf("Insira o ficheiro a utilizar\n");
    printf("\n5-tourism_5.txt");
    printf("\n20-tourism_20.txt");
    printf("\n50-tourism_50.txt");
    printf("\n100-tourism_100.txt");
    printf("\n250 tourism_250.txt");
    printf("\n500-tourism_500.txt");

    printf("\n\n>");

    scanf("%s",escolha);

    //ATENCAO, COMVEM TIRAR ISTO DEPOIS

    if(strcmp("5",escolha)==0)
        strcpy(fich,"tourism_5.txt");
    else if(strcmp("20",escolha)==0)
        strcpy(fich,"tourism_20.txt");
    else if(strcmp("50",escolha)==0)
        strcpy(fich,"tourism_50.txt");
    else if(strcmp("100",escolha)==0)
        strcpy(fich,"tourism_100.txt");
    else if(strcmp("250",escolha)==0)
        strcpy(fich,"tourism_250.txt");
    else if(strcmp("500",escolha)==0)
        strcpy(fich,"tourism_500.txt");
    else
        strcpy(fich,escolha);


    printf("\nficheiro: %s\n",fich);

    FILE *f = fopen(fich, "r");

    if (f == NULL) {
        printf("Esse ficheiro nao existe");
        return 1;
    }

    fscanf(f,"%d %d\n",nC,m);

    for(int i=0; i<*nC; i++)
        for(int j=0; j<*nC; j++)
            distancias[i][j] = 0.0;


    // Le as arestas (e1 e2 50.1...)
    char no1[10], no2[10];
    float dist;

    while (fscanf(f, "%s %s %f", no1, no2, &dist) == 3) {
        // 'no1' e string "e1", no1[1] e o '1' da string

        int id1 = atoi(&no1[1]) - 1;
        int id2 = atoi(&no2[1]) - 1;


        if (id1 >= 0 && id1 < *nC && id2 >= 0 && id2 < *nC) {
            distancias[id1][id2] = dist;
            distancias[id2][id1] = dist; // A matriz e simetrica
        }
    }

    fclose(f);

    // so para testar
    // mostraMatrizDistancias(distancias,*nC);

    return 0;
}

int geraNumEntre(int inf, int sup) {
    return inf + rand() % (sup-inf+1);
}

void copiaSolucao(Solucao* destino, Solucao* origem, int nC) {
    for (int i = 0; i < nC; i++) {
        destino->sel[i] = origem->sel[i];
    }
    destino->media = origem->media;
    destino->nSel = origem->nSel;
}

float calculaMaxDistancia(float distancias[MAX_C][MAX_C], int nC) {
    float max = 0.0;
    for (int i = 0; i < nC; i++) {
        for (int j = i + 1; j < nC; j++) {
            if (distancias[i][j] > max) {
                max = distancias[i][j];
            }
        }
    }
    return max;
}

float calculaValorPenalizacao(int nSel, int m, float valPenalidade) {
    if (nSel != m) {
        return abs(nSel - m) * valPenalidade;
    }
    return 0.0f;
}

void mostraRelatorioPenalizacao(float penAplicada, float fitnessBase) {
    float percentagem = 0.0f;

    if (fitnessBase > 0.0001f) {
        percentagem = (penAplicada / fitnessBase) * 100.0f;
    }
    printf("Percentagem de invalidos (%.2f%%)", percentagem);
}


float random_0_1() {
    return (float)rand() / (float)RAND_MAX;
}
