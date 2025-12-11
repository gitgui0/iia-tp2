#include "funcao.h"
#include "algoritmo.h"
#include "proc.h"

#include <windows.h>
#include <unistd.h>

int main() {
    int nC,m,numRuns=-1, numIter;
    float distancias[MAX_C][MAX_C], mbf=0;

    int tipoVizinhanca=1; // default para o trepacolinas
    int aceitaMesmoCusto;
    int numIterLocal;
    char algoritmoEscolhido;
    int algoritmoHibrido, abordagemHibrido;
    Solucao* sol;
    Solucao* melhorSol;

    float temperaturaMaxima, arrefecimento, temperaturaMinima;

    Evolutivo ev;

    Solucao *pop = NULL;
    Solucao *pais = NULL;
    Solucao *filhos = NULL;

    printf("\n| TP IIA - Problema de Optimizacao |\n");

    if (leInfo(&nC,&m,distancias)!=0) {
        printf("\nErro ao ler informacoes.");
        return 1;
    }

    float maxDist = calculaMaxDistancia(distancias, nC);
    //printf("Distancia Maxima encontrada no ficheiro: %.2f\n", maxDist);
    float pen = maxDist * 10.0;
    //float pen = 0.0;

    srand(time(NULL));

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

    while (algoritmoEscolhido != 'r' && algoritmoEscolhido != 't' && algoritmoEscolhido != 'e' && algoritmoEscolhido != 'h') {
        printf("\n| Escolha o Algoritmo |\n\nt-Trepa colinas\nr-Recristalizacao Simulada\ne-Evolutivo\nh-Hibrido\n>");
        scanf(" %c",&algoritmoEscolhido);
    }

    if (algoritmoEscolhido=='r') {
        if (lerParametrosRecristalizacao(&temperaturaMaxima,&arrefecimento,&temperaturaMinima,&tipoVizinhanca,&aceitaMesmoCusto) == 1) {
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
    }else if (algoritmoEscolhido == 'h'){
        if (lerParametrosHibrido(&ev,&algoritmoHibrido,&abordagemHibrido,&temperaturaMaxima,&arrefecimento,&temperaturaMinima,&numIterLocal, &aceitaMesmoCusto, &tipoVizinhanca) == 1) {
            printf("\nErro a ler os parametros para o hibrido.");
            return 1;
        }

        pop = malloc(sizeof(Solucao) * ev.popsize);
        pais = malloc(sizeof(Solucao) * ev.popsize);
        filhos = malloc(sizeof(Solucao) * ev.popsize);
        if (!pop || !pais || !filhos) { printf("Erro memoria populacoes\n"); return 1; }
    }


    // --- ZONA DE PRINTS DOS PARÂMETROS ESCOLHIDOS  ---

    printf("\n==================================================");
    printf("\nRESUMO DOS PARAMETROS ESCOLHIDOS");
    printf("\n==================================================");
    printf("\nNumero de Runs: %d", numRuns);
    printf("\nNumero de Iteracoes (Globais): %d", numIter);

    if (algoritmoEscolhido == 't') {
        printf("\n\n--- ALGORITMO: TREPA COLINAS ---");
        printf("\nTipo Vizinhanca: %s", (tipoVizinhanca == 1 ? "Troca" : "BitFlip"));
    }
    else if (algoritmoEscolhido == 'r') {
        printf("\n\n--- ALGORITMO: RECRISTALIZACAO SIMULADA ---");
        printf("\nTemp Maxima: %.5f", temperaturaMaxima);
        printf("\nTemp Minima: %.5f", temperaturaMinima);
        printf("\nArrefecimento: %.5f", arrefecimento);
        printf("\nTipo Vizinhanca: %s", (tipoVizinhanca == 1 ? "Troca" : "BitFlip"));
        printf("\nAceita Vizinho Mesmo Custo: %s", (aceitaMesmoCusto == 1 ? "Sim" : "Nao"));
    }
    else if (algoritmoEscolhido == 'e' || algoritmoEscolhido == 'h') {
        if (algoritmoEscolhido == 'e') printf("\n\n--- ALGORITMO: EVOLUTIVO ---");
        else printf("\n\n--- ALGORITMO: HIBRIDO ---");

        printf("\n\n[Parametros Evolutivos]");
        printf("\nTamanho Populacao: %d", ev.popsize);
        printf("\nProb Mutacao: %.4f", ev.pmt);
        printf("\nProb Recombinacao: %.4f", ev.prc);
        printf("\nTamanho Torneio: %d", ev.tsize);

        // Traduzir Códigos para Texto
        char rec[30], mut[30], inv[30], sel[30];

        if (ev.tipoRecombinacao == 1) strcpy(rec, "1 Ponto");
        else if (ev.tipoRecombinacao == 2) strcpy(rec, "2 Pontos");
        else strcpy(rec, "Uniforme");

        if (ev.tipoMutacao == 1) strcpy(mut, "Bitflip");
        else strcpy(mut, "Troca");

        if (ev.tipoReparacao == 0) strcpy(inv, "Penalizacao");
        else if (ev.tipoReparacao == 1) strcpy(inv, "Rep. Aleatoria");
        else strcpy(inv, "Rep. Heuristica");

        if (ev.metodoSelecao == 1) strcpy(sel, "Torneio");
        else strcpy(sel, "Roleta");

        printf("\nTipo Recombinacao: %s", rec);
        printf("\nTipo Mutacao: %s", mut);
        printf("\nTratamento Invalidos: %s", inv);
        printf("\nMetodo de Selecao: %s", sel);

        // SECÇÃO ESPECÍFICA DO HÍBRIDO
        if (algoritmoEscolhido == 'h') {
            printf("\n\n[Parametros Hibrido - Pesquisa Local]");
            printf("\nAbordagem: %d", abordagemHibrido);
            printf("\nAlgoritmo Local: %s", (algoritmoHibrido == 1 ? "Trepa Colinas" : "Recristalizacao Simulada"));
            printf("\nIteracoes Locais: %d", numIterLocal);
            printf("\nTipo Vizinhanca Local: %s", (tipoVizinhanca == 1 ? "Troca" : "BitFlip"));

            if (algoritmoHibrido == 2) { // Se for Recristalização
                printf("\nTemp Maxima: %.5f", temperaturaMaxima);
                printf("\nTemp Minima: %.5f", temperaturaMinima);
                printf("\nArrefecimento: %.5f", arrefecimento);
                printf("\nAceita Mesmo Custo: %s", (aceitaMesmoCusto == 1 ? "Sim" : "Nao"));
            }
        }
    }
    printf("\n==================================================\n\n");

    sleep(2); // para dar tempo para ver os prints de cima

    melhorSol->media = -1; // so para comecar
    for (int i = 0; i < numRuns; i++) {
        geraSolucaoInicial(sol, m, nC);
        int countValidos = 1;

        int numIterTemp = numIter; // a recristalizacao pode muda lo

        if (algoritmoEscolhido == 't') {
            if (trepaColinas(sol,distancias,m,nC,&numIterTemp,pen,&countValidos, tipoVizinhanca)!=0) {
                printf("ERRO NO ALGORITMO TREPA-COLINAS\n");
                return 1;
            }
        }else if (algoritmoEscolhido == 'r') {

            if (recristalizacao(sol,distancias,m,nC,&numIterTemp,pen,&countValidos,temperaturaMaxima,arrefecimento,temperaturaMinima, tipoVizinhanca, aceitaMesmoCusto)!=0) {
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
        else if (algoritmoEscolhido == 'h') {
            // A função escreve a melhor solução da run diretamente em 'sol'
            geraSolucaoHibrido(sol, distancias, nC, m, pen, ev, pop, pais, filhos, abordagemHibrido, algoritmoHibrido, temperaturaMaxima, arrefecimento, temperaturaMinima, numIterLocal, tipoVizinhanca, aceitaMesmoCusto);

            //assumimos que a solucao final e válida se usarmos reparação
            if (ev.tipoReparacao > 0)
                countValidos = numIterTemp + 1;
        }

        printf("\nRepeticao %d\n",i+1);
        printSol(sol,nC);
        printf("\nMedia de Distancias: %.2f \n",sol->media);
        printf("\nNumero de Iteracoes: %d\n",numIterTemp);

        printf("Percentagem de Invalidos: %.2f\n", ( 1.0 - (float)countValidos / ( (float)numIterTemp + 1.0 ) ) * 100);

        mbf+=sol->media;

        if (sol->media > melhorSol->media) {
            copiaSolucao(melhorSol,sol, nC);
        }
    }

    printf("\n----MELHOR SOLUCAO ENCONTRADA----\n");
    printSol(melhorSol,nC);
    printf("\nMelhor Solucao - Media de Distancias: %.2f \n",melhorSol->media);
    printf("\nMelhor Solucao - MBF: %.2f\n",mbf/(float)numRuns);

    free(sol);
    free(melhorSol);

    if (algoritmoEscolhido == 'e' || algoritmoEscolhido == 'h') {
        free(pop);
        free(pais);
        free(filhos);
    }

    return 0;
}