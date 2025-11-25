#include "funcao.h"
#include "algoritmo.h"



int main() {
    int nC,m,numRuns=-1, numIter;
    float distancias[MAX_C][MAX_C], mbf=0;
    Solucao* sol;
    Solucao* melhorSol;

    if (leInfo(&nC,&m,distancias)!=0) {
        printf("\nErro o ler informacoes.");
        return 1;
    }

    srand(time(NULL));

    // GERAR SOLUCAO

    sol = malloc(sizeof(Solucao));
    if (sol == NULL) {
        printf("Erro ao alocar solucao.\n");
        return 1;
    }

    melhorSol = malloc(sizeof(Solucao));
    if (melhorSol == NULL) {
        free(sol);
        printf("Erro ao alocar melhorSol.\n");
        return 1;
    }

    do {
        printf("\nNumero de runs -> ");
        scanf("%d",&numRuns);
    }while (numRuns<=0);

    do {
        printf("\nNumero de iteracoes para o algoritmo trepa-colinas -> ");
        scanf("%d",&numIter);
    }while (numIter<=0);

    printf("\n---ALGORTIMO TREPA-COLINAS---\n");

    melhorSol->media = -1; // so para comecar

    for (int i = 0; i < numRuns; i++) {
        geraSolucaoInicial(sol, m, nC);
        sol->media = devolveMedia(sol,distancias,nC,m);
        sol->nSel = m; // PARA O TREPA COLINAS

        if (trepaColinas(sol,distancias,m,nC,numIter)!=0) {
            printf("ERRO NO ALGORITMO TREPA-COLINAS\n");
            return 1;
        }
        printf("\nRepeticao %d\n",i+1);
        printSol(sol,nC);
        printf("\nMedia de Distancias: %.2f \n",sol->media);

        mbf+=sol->media;

        if (sol->media > melhorSol->media) {
            copiaSolucao(melhorSol,sol, nC);
        }
    }

    printf("\n----MELHOR SOLUCAO ENCONTRADA----\n");
    printSol(melhorSol,nC);
    printf("\nMedia de Distancias: %.2f \n",melhorSol->media);
    printf("\nMBF: %.2f\n",mbf/(float)numRuns);

    free(sol);
    free(melhorSol);

    return 0;
}