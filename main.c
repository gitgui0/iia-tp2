    #include "funcao.h"
    #include "algoritmo.h"



int main() {
    int nC,m,numRuns=-1, numIter;
    float distancias[MAX_C][MAX_C], mbf=0;

    char algoritmoEscolhido;
    Solucao* sol;
    Solucao* melhorSol;

    float temperatura, arrefecimento,temperaturaFinal;

    Evolutivo ev;

    Solucao *pop = NULL;
    Solucao *pais = NULL;
    Solucao *filhos = NULL;

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

    do {
        printf("\nNumero de runs -> ");
        scanf("%d",&numRuns);
    }while (numRuns<=0);

    do {
        printf("\nNumero de iteracoes para o algoritmo -> ");
        scanf("%d",&numIter);
    }while (numIter<=0);

    while (algoritmoEscolhido != 'r' && algoritmoEscolhido != 't' && algoritmoEscolhido != 'e') {
        printf("\nLetra do algoritmo a usar\nt-trepa colinas\tr-recristalizacao simulada\t e-Evolutivo -> ");
        scanf(" %c",&algoritmoEscolhido);
    }


    if (algoritmoEscolhido=='r') {
        printf("Temperatura Inicial para Recristalizacao Simulada->");
        scanf("%f",&temperatura);

        printf("Temperatura Final para Recristalizacao Simulada->");
        scanf("%f",&temperaturaFinal);
        do {
            printf("Fator de arrefecimento para Recristalizacao Simulada->");
            scanf("%f",&arrefecimento);
        }while (arrefecimento<= 0 || arrefecimento >= 1);
    }else if (algoritmoEscolhido == 'e'){
        printf("\n--- CONFIGURACAO EVOLUTIVO ---\n");
        printf("Tamanho da Populacao (ex: 50, 100): ");
        scanf("%d", &ev.popsize);

        printf("Prob. Mutacao (ex: 0.01): ");
        scanf("%f", &ev.pmt);

        printf("Prob. Recombinacao (ex: 0.7): ");
        scanf("%f", &ev.prc);

        printf("Tamanho Torneio (ex: 2): ");
        scanf("%d", &ev.tsize);

        printf("\nTipo Recombinacao (1-1Ponto, 2-2Pontos, 3-Uniforme): ");
        scanf("%d", &ev.tipoRecombinacao);

        printf("Tipo Mutacao (1-BitFlip, 2-Troca): ");
        scanf("%d", &ev.tipoMutacao);

        printf("Tratamento Invalidos (0-Penalizacao, 1-Rep. Aleatoria, 2-Rep. Heuristica): ");
        scanf("%d", &ev.tipoReparacao);
        ev.numGenerations = numIter;

        pop = malloc(sizeof(Solucao) * ev.popsize);
        pais = malloc(sizeof(Solucao) * ev.popsize);
        filhos = malloc(sizeof(Solucao) * ev.popsize);
        if (!pop || !pais || !filhos) { printf("Erro memoria populacoes\n"); return 1; }
    }

    melhorSol->media = -1; // so para comecar

    if (algoritmoEscolhido == 't') printf("\n---ALGORTIMO TREPA COLINAS ---\n");
    else if (algoritmoEscolhido == 'r') printf("\n---ALGORTIMO RECRISTALIZACAO SIMULADA---\n");
    else if (algoritmoEscolhido == 'e') printf("\n--- ALGORITMO EVOLUTIVO ---\n");

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