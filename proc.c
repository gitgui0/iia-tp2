#include "algoritmo.h"
#include "proc.h"
#include <stdio.h>


int lerParametrosRecristalizacao(float* temperaturaInicial, float* arrefecimento,float* temperaturaFinal, int* vizinhanca, int *aceita){
  int escolha;

  printf("Como deseja os parametros?\n\n0-ficheiro - params_recristalizacao.txt \n1-hardcoded\n2-manualmente\n\n>");
  scanf("%d",&escolha);

  if(escolha == 0){
    FILE* f = fopen("params_recristalizacao.txt","r");
    if(f==NULL){
      printf("\nNao foi encontrado o ficheiro params_recristalizacao.txt");
      return 1;
    }
    fscanf(f,"TempInicial = %f\n",temperaturaInicial);
    fscanf(f,"TempFinal = %f\n",temperaturaFinal);
    fscanf(f,"Arrefecimento = %f\n",arrefecimento);
    fscanf(f,"Vizinhanca = %d\n",vizinhanca);
    fscanf(f,"AceitaMesmo = %d\n",aceita);

    if(*arrefecimento<= 0 || *arrefecimento >= 1){
      printf("\nArrefecimento invalido");
      return 1;
    }

    fclose(f);
  }
  if(escolha == 2){

    printf("Temperatura Inicial para Recristalizacao Simulada->");
    scanf("%f",temperaturaInicial);

    printf("Temperatura Final para Recristalizacao Simulada->");
    scanf("%f",temperaturaFinal);

    do {
      printf("Fator de arrefecimento para Recristalizacao Simulada->");
      scanf("%f",arrefecimento);
    }while (*arrefecimento<= 0 || *arrefecimento >= 1);

    printf("Vizinhanca->");
    scanf("%d",vizinhanca);

    printf("Aceita solucoes do mesmo custo->");
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

  }




  return 0;
}


int lerParametrosHibrido(Evolutivo *ev, int *algoritmo, int* abordagem, float* temperaturaInicial, float* arrefecimento,float* temperaturaFinal){
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

    if (*algoritmo == 2) {
      fscanf(f,"TempInicial = %f\n",temperaturaInicial);
      fscanf(f,"TempFinal = %f\n",temperaturaFinal);
      fscanf(f,"Arrefecimento = %f\n",arrefecimento);

      if(*arrefecimento<= 0 || *arrefecimento >= 1){
        printf("\nArrefecimento invalido");
        return 1;
      }
    }


    fclose(f);
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

    printf("Algoritmo (1-Trepa Colinas, 2-Recristalizacao Simulada): ");
    scanf("%d", algoritmo);

    if (*algoritmo == 2) {
        printf("Tratamento Invalidos (0-Penalizacao, 1-Rep. Aleatoria, 2-Rep. Heuristica): ");
        scanf("%d", &ev->tipoReparacao);

        printf("Temperatura Inicial para Recristalizacao Simulada->");
        scanf("%f",temperaturaInicial);

        printf("Temperatura Final para Recristalizacao Simulada->");
        scanf("%f",temperaturaFinal);

        do {
          printf("Fator de arrefecimento para Recristalizacao Simulada->");
          scanf("%f",arrefecimento);
        }while (*arrefecimento<= 0 || *arrefecimento >= 1);
    }

  }
  return 0;
}


