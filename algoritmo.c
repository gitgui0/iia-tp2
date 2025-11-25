
#include "algoritmo.h"
#include "funcao.h"

//Troca 1 bit por outro. Mantem a validade da solucao
void geraVizinho(Solucao *atual, Solucao* nova, int nC, int m){
    int p1, p2;

    //Copia a atual para a nova
    for(int i = 0; i < nC; i++){
        nova->sel[i] = atual->sel[i];
    }

    //escolher um a 0
    do{
        p1=geraNumEntre(0,nC-1);
    }while(nova->sel[p1] != 0);

    //escolher um a 1
    do{
        p2=geraNumEntre(0,nC-1);
    }while(nova->sel[p2] != 1 || p1==p2);

    //Troca
    nova->sel[p1] = 1;
    nova->sel[p2] = 0;

    // neste caso
    nova->nSel = m;
}

// 1 -> erro, 0 -> tudo certo
int trepaColinas(Solucao *atual, float distancias[MAX_C][MAX_C],int m, int nC, int numIter){
    Solucao* vizinho;

    vizinho = malloc(sizeof(Solucao));
    if(vizinho == NULL){
      printf("Erro ao alocar nova solucao.");
      return 1;
    }

    //Avalia solucao inicial
    atual->media = devolveMedia(atual,distancias,nC,m);

    for(int i = 0; i < numIter; i++){
        geraVizinho(atual,vizinho,nC, m);
        vizinho->media = devolveMedia(vizinho,distancias,nC,m);

        if(vizinho->media > atual->media){
            // Copia a solucao vizinho para a solucao atual, incluindo a media de distancias
            copiaSolucao(atual,vizinho,nC);
        }
        //else?
    }

    free(vizinho);
    return 0;
}

