    #include "funcao.h"
    #include "algoritmo.h"



    int main() {
        int nC,m,numRuns=-1, numIter;
        float distancias[MAX_C][MAX_C], mbf=0;

        char algoritmoEscolhido;
        Solucao* sol;
        Solucao* melhorSol;

        float temperatura;
        float arrefecimento;
        float temperaturaFinal;

        if (leInfo(&nC,&m,distancias)!=0) {
            printf("\nErro o ler informacoes.");
            return 1;
        }

        float maxDist = calculaMaxDistancia(distancias, nC);
        //printf("Distancia Maxima encontrada no ficheiro: %.2f\n", maxDist);
        float pen = maxDist * 10.0;

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


        printf("\nLetra do algoritmo a usar\nt-trepa colinas\tr-recristalizacao simulada -> ");
        scanf("%c",&algoritmoEscolhido);
        while (algoritmoEscolhido != 'r' && algoritmoEscolhido != 't') {
            printf("\nLetra do algoritmo a usar\nt-trepa colinas\tr-recristalizacao simulada -> ");
            scanf("%c",&algoritmoEscolhido);
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

        }

        melhorSol->media = -1; // so para comecar

        if (algoritmoEscolhido == 't') printf("\n---ALGORTIMO TREPA COLINAS ---\n");
        else if (algoritmoEscolhido == 'r') printf("\n---ALGORTIMO RECRISTALIZACAO SIMULADA---\n");

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
            }else {
                break;
            }
            printf("\nRepeticao %d\n",i+1);
            printSol(sol,nC);
            printf("\nMedia de Distancias: %.2f \n",sol->media);


            printf("Percentagem de Invalidos: %.2f", ( 1.0 - (float)countValidos / ( (float)numIterTemp + 1.0 ) ) * 100);

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