#include "funcao.h"
#include "algoritmo.h"
#include "proc.h"

#include <windows.h>
#include <unistd.h>

int main() {
    int nC,m,numRuns=-1, numIter;
    float distancias[MAX_C][MAX_C], mbf=0;

    char algoritmoEscolhido;
    Solucao* sol;
    Solucao* melhorSol;

    float temperatura = 1000, arrefecimento = 0.99, temperaturaFinal = 1;

    Evolutivo ev;

    /*
        TIPO RECOMBINACAO - 1 - 1Ponto, 2 - 2Pontos, 3 - Uniforme
        TIPO MUTACAO - 1 - BitFlip, 2 - Troca
        TRATAMENTO INVALIDOS (0-Penalizacao, 1-Rep. Aleatoria, 2-Rep. Heuristica)
    */

    ev.popsize = 50;
    ev.pmt = 0.01;
    ev.prc = 0.7;
    ev.tsize = 2;
    ev.tipoRecombinacao  = 1;
    ev.tipoMutacao = 1;
    ev.tipoReparacao = 0;

    Solucao *pop = NULL;
    Solucao *pais = NULL;
    Solucao *filhos = NULL;

    printf("\n| TP IIA - Problema de Optimizacao |\n");

    if (leInfo(&nC,&m,distancias)!=0) {
        printf("\nErro o ler informacoes.");
        return 1;
    }

    float maxDist = calculaMaxDistancia(distancias, nC);
    //printf("Distancia Maxima encontrada no ficheiro: %.2f\n", maxDist);
    float pen = maxDist * 10.0;
    //float pen = 0.0;

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

    printf("\n| Numero de Runs e de Iteracoes |\n");

    do {
        printf("\nNumero de runs -> ");
        scanf("%d",&numRuns);
    }while (numRuns<=0);

    do {
        printf("Numero de iteracoes para o algoritmo -> ");
        scanf("%d",&numIter);
    }while (numIter<=0);

    while (algoritmoEscolhido != 'r' && algoritmoEscolhido != 't' && algoritmoEscolhido != 'e') {
        printf("\n| Escolha o Algoritmo |\n\nt-Trepa colinas\nr-Recristalizacao Simulada\ne-Evolutivo\n>");
        scanf(" %c",&algoritmoEscolhido);
    }



    if (algoritmoEscolhido=='r') {
        if (lerParametrosRecristalizacao(&temperatura,&arrefecimento,&temperaturaFinal) == 1) {
            printf("\nErro a ler os parametros para recristalizacao.");
            return 1;
        }
    }else if (algoritmoEscolhido == 'e'){
        if (lerParametrosEvolutivo(&ev) == 1) {
            printf("\nErro a ler os parametros para o evolutivo.");
            return 1;
        }

        pop = malloc(sizeof(Solucao) * ev.popsize);
        pais = malloc(sizeof(Solucao) * ev.popsize);
        filhos = malloc(sizeof(Solucao) * ev.popsize);
        if (!pop || !pais || !filhos) { printf("Erro memoria populacoes\n"); return 1; }
    }

    melhorSol->media = -1; // so para comecar

    if (algoritmoEscolhido == 't') printf("\n---ALGORTIMO TREPA COLINAS ---\n");
    else if (algoritmoEscolhido == 'r') printf("\n---ALGORTIMO RECRISTALIZACAO SIMULADA---\nParametros: \n\nTempInicial = %.5f \nArrefecimento = %.5f \nTempFinal = %.5f\n\n",temperatura,arrefecimento,temperaturaFinal);
    else if (algoritmoEscolhido == 'e') {
        printf("\n--- ALGORITMO EVOLUTIVO ---\nParametros: \n\n");
        printf("Tamanho Populacao = %d\n",ev.popsize);
        printf("Prob Mutacao = %f\n",ev.pmt);
        printf("Prob Recombinacao = %f\n",ev.prc);
        printf("Tamanho Torneio = %d\n\n",ev.tsize );

        char rec[30];
        char mut[30];
        char inv[30];

        if (ev.tipoRecombinacao == 1)
            strcpy(rec,"1 Ponto");
        if (ev.tipoRecombinacao == 2)
            strcpy(rec,"2 Pontos");
        if (ev.tipoRecombinacao == 3)
            strcpy(rec,"Uniforme");

        if (ev.tipoMutacao == 1)
            strcpy(mut,"Bitflip");
        if (ev.tipoMutacao == 2)
            strcpy(mut,"Troca");

        if (ev.tipoReparacao == 0)
            strcpy(inv,"Penalizacao");
        if (ev.tipoReparacao == 1)
            strcpy(inv,"Rep. Aleatoria");
        if (ev.tipoReparacao == 2)
            strcpy(inv,"Rep. Heuristica");

        printf("Tipo Recombinacao - %s\n",rec );
        printf("Tipo Mutacao - %s\n",mut );
        printf("Tratamento Invalidos - %s\n\n",inv );
    }

    sleep(2);
    for (int i = 0; i < numRuns; i++) {
        geraSolucaoInicial(sol, m, nC);
        int countValidos = 1;

        int numIterTemp = numIter; // a recristalizacao pode muda lo

        if (algoritmoEscolhido == 't') {
            if (trepaColinas(sol,distancias,m,nC,&numIterTemp,pen,&countValidos)!=0) {
                printf("ERRO NO ALGORITMO TREPA-COLINAS\n");
                return 1;
            }
        }else if (algoritmoEscolhido == 'r') {

            if (recristalizacao(sol,distancias,m,nC,&numIterTemp,pen,&countValidos,temperatura,arrefecimento,temperaturaFinal)!=0) {
                printf("ERRO NO ALGORITMO RECRISTALIZACAO SIMULADA\n");
                return 1;
            }
        }else if (algoritmoEscolhido == 'e') {
            // A função escreve a melhor solução da run diretamente em 'sol'
            geraSolucaoEvolutivo(sol, distancias, nC, m, pen, ev, pop, pais, filhos);

            //assumimos que a solucao final e válida se usarmos reparação
            if (ev.tipoReparacao > 0)
                countValidos = numIterTemp + 1;
        }

        printf("\nRepeticao %d\n",i+1);
        printSol(sol,nC);
        printf("\nMedia de Distancias: %.2f \n",sol->media);


        printf("Percentagem de Invalidos: %.2f\n", ( 1.0 - (float)countValidos / ( (float)numIterTemp + 1.0 ) ) * 100);

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
    if (algoritmoEscolhido == 'e') {
        free(pop); free(pais); free(filhos);
    }

    return 0;
}