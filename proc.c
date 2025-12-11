#include "algoritmo.h"
#include "proc.h"
#include <stdio.h>


int lerParametrosRecristalizacao(float* temperaturaMaxima, float* arrefecimento,float* temperaturaMinima, int* vizinhanca, int *aceita){
  int escolha;

  printf("Como deseja os parametros?\n\n0-ficheiro - params_recristalizacao.txt \n1-hardcoded\n2-manualmente\n\n>");
  scanf("%d",&escolha);

  if(escolha == 0){
    FILE* f = fopen("params_recristalizacao.txt","r");
    if(f==NULL){
      printf("\nNao foi encontrado o ficheiro params_recristalizacao.txt");
      return 1;
    }
    fscanf(f,"TempMax = %f\n",temperaturaMaxima);
    fscanf(f,"TempMin = %f\n",temperaturaMinima);
    fscanf(f,"Arrefecimento = %f\n",arrefecimento);
    fscanf(f,"Vizinhanca = %d\n",vizinhanca);
    fscanf(f,"AceitaMesmo = %d\n",aceita);

    if(*arrefecimento<= 0 || *arrefecimento >= 1){
      printf("\nArrefecimento invalido");
      return 1;
    }

    fclose(f);
  }
  else if(escolha == 1){
    *temperaturaMaxima = 1000.0;
    *temperaturaMinima = 0.01;
    *arrefecimento = 0.99;
    *vizinhanca = 1;
    *aceita = 0;
  }
  else if(escolha == 2){
    printf("Temperatura Maxima para Recristalizacao Simulada->");
    scanf("%f",temperaturaMaxima);
    printf("Temperatura Minima para Recristalizacao Simulada->");
    scanf("%f",temperaturaMinima);
    do {
      printf("Fator de arrefecimento para Recristalizacao Simulada->");
      scanf("%f",arrefecimento);
    }while (*arrefecimento<= 0 || *arrefecimento >= 1);
    printf("Vizinhanca->");
    scanf("%d",vizinhanca);
    printf("Aceita solucoes do mesmo custo (0,1)->");
    scanf("%d",aceita);
  }
  return 0;
};


int lerParametrosEvolutivo(Evolutivo *ev){
  int escolha;
  printf("Como deseja os parametros?\n\n0-ficheiro - params_evolutivo.txt \n1-hardcoded\n2-manualmente\n\n>");
  scanf("%d",&escolha);

  if(escolha == 0){
    FILE* f = fopen("params_evolutivo.txt","r");
    if(f==NULL){
      printf("\nNao foi encontrado o ficheiro params_evolutivo.txt");
      return 1;
    }

    fscanf(f,"Tamanho Populacao = %d\n",&ev->popsize);
    fscanf(f,"Prob Mutacao = %f\n",&ev->pmt);
    fscanf(f,"Prob Recombinacao = %f\n",&ev->prc);
    fscanf(f,"Tamanho Torneio = %d\n",&ev->tsize );
    fscanf(f,"Tipo Recombinacao = %d\n",&ev->tipoRecombinacao );
    fscanf(f,"Tipo Mutacao = %d\n",&ev->tipoMutacao );
    fscanf(f,"Tratamento Invalidos = %d\n",&ev->tipoReparacao );
    fscanf(f,"Metodo Selecao = %d\n",&ev->metodoSelecao );

    fclose(f);
  }
  if (escolha == 1) {
      ev->popsize = 100;
      ev->pmt = 0.01;
      ev->prc = 0.7;
      ev->tsize = 2;
      ev->tipoRecombinacao = 1;
      ev->tipoMutacao = 1;
      ev->tipoReparacao = 0;
      ev->metodoSelecao = 1;
  }

  if(escolha == 2){

    printf("Tamanho da Populacao (ex: 50, 100): ");
    scanf("%d", &ev->popsize);

    printf("Prob. Mutacao (ex: 0.01): ");
    scanf("%f", &ev->pmt);

    printf("Prob. Recombinacao (ex: 0.7): ");
    scanf("%f", &ev->prc);

    printf("Tamanho Torneio (ex: 2): ");
    scanf("%d", &ev->tsize);

    printf("\nTipo Recombinacao (1-1Ponto, 2-2Pontos, 3-Uniforme): ");
    scanf("%d", &ev->tipoRecombinacao);

    printf("Tipo Mutacao (1-BitFlip, 2-Troca): ");
    scanf("%d", &ev->tipoMutacao);

    printf("Tratamento Invalidos (0-Penalizacao, 1-Rep. Aleatoria, 2-Rep. Heuristica): ");
    scanf("%d", &ev->tipoReparacao);

    printf("Metodo de Selecao  (1-Torneio, 2-Roleta): ");
    scanf("%d", &ev->metodoSelecao);

  }




  return 0;
}


int lerParametrosHibrido(Evolutivo *ev, int *algoritmo, int* abordagem, float* temperaturaMaxima, float* arrefecimento,float* temperaturaMinima, int* numIterLocal, int* aceita, int* vizinhanca){
  int escolha;
  printf("Como deseja os parametros?\n\n0-ficheiro - params_hibrido.txt \n1-hardcoded\n2-manualmente\n\n>");
  scanf("%d",&escolha);

  if(escolha == 0){
    FILE* f = fopen("params_hibrido.txt","r");
    if(f==NULL){
      printf("\nNao foi encontrado o ficheiro params_hibrido.txt");
      return 1;
    }

    fscanf(f,"Tamanho Populacao = %d\n",&ev->popsize);
    fscanf(f,"Prob Mutacao = %f\n",&ev->pmt);
    fscanf(f,"Prob Recombinacao = %f\n",&ev->prc);
    fscanf(f,"Tamanho Torneio = %d\n",&ev->tsize );
    fscanf(f,"Tipo Recombinacao = %d\n",&ev->tipoRecombinacao );
    fscanf(f,"Tipo Mutacao = %d\n",&ev->tipoMutacao );
    fscanf(f,"Tratamento Invalidos = %d\n",&ev->tipoReparacao );
    fscanf(f,"Metodo Selecao = %d\n",&ev->metodoSelecao );
    fscanf(f,"Algoritmo Local = %d\n",algoritmo );
    fscanf(f,"Abordagem = %d\n",abordagem );
    fscanf(f, "Vizinhanca = %d\n",vizinhanca);


    if (*algoritmo == 2) {
      fscanf(f,"TempMax = %f\n",temperaturaMaxima);
      fscanf(f,"TempMin = %f\n",temperaturaMinima);
      fscanf(f,"Arrefecimento = %f\n",arrefecimento);

      if(*arrefecimento<= 0 || *arrefecimento >= 1){
        printf("\nArrefecimento invalido");
        return 1;
      }
    }
    fscanf(f,"Num iter = %d\n",numIterLocal);

    fclose(f);
  }
  if(escolha == 1){
    ev->popsize = 50;
    ev->pmt = 0.01;
    ev->prc = 0.7;
    ev->tsize = 2;
    ev->tipoRecombinacao = 1;
    ev->tipoMutacao = 1;
    ev->tipoReparacao = 0;
    ev->metodoSelecao = 1;
    *algoritmo = 2; // Recristalizacao
    *abordagem = 2;
    *temperaturaMaxima = 1000;
    *temperaturaMinima = 1;
    *arrefecimento = 0.9;
    *numIterLocal = 15;
    *aceita = 0;
    *vizinhanca = 1;
  }
  if(escolha == 2){

    printf("Tamanho da Populacao (ex: 50, 100): ");
    scanf("%d", &ev->popsize);

    printf("Prob. Mutacao (ex: 0.01): ");
    scanf("%f", &ev->pmt);

    printf("Prob. Recombinacao (ex: 0.7): ");
    scanf("%f", &ev->prc);

    printf("Tamanho Torneio (ex: 2): ");
    scanf("%d", &ev->tsize);

    printf("\nTipo Recombinacao (1-1Ponto, 2-2Pontos, 3-Uniforme): ");
    scanf("%d", &ev->tipoRecombinacao);

    printf("Tipo Mutacao (1-BitFlip, 2-Troca): ");
    scanf("%d", &ev->tipoMutacao);

    printf("Tratamento Invalidos (0-Penalizacao, 1-Rep. Aleatoria, 2-Rep. Heuristica): ");
    scanf("%d", &ev->tipoReparacao);

    printf("Abordagem (1 ou 2)->");
    scanf("%d", abordagem);

    printf("Vizinhanca (1-troca ou 2-bitflip)->");
    scanf("%d", vizinhanca);

    printf("Algoritmo (1-Trepa Colinas, 2-Recristalizacao Simulada): ");
    scanf("%d", algoritmo);

    if (*algoritmo == 2) {

        printf("Temperatura Maxima para Recristalizacao Simulada->");
        scanf("%f",temperaturaMaxima);

        printf("Temperatura Minima para Recristalizacao Simulada->");
        scanf("%f",temperaturaMinima);

        do {
          printf("Fator de arrefecimento para Recristalizacao Simulada->");
          scanf("%f",arrefecimento);
        }while (*arrefecimento<= 0 || *arrefecimento >= 1);
    }

    printf("Numero de iteracoes para o algoritmo local: ");
    scanf("%d", numIterLocal);

    printf("Aceita solucoes do mesmo custo (nao-0, sim-1)->");
    scanf("%d",aceita);

  }
  return 0;
}


