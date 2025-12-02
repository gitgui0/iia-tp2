
#include "algoritmo.h"
#include "funcao.h"
#include "math.h"

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


// escolhe um local aleatorio e inverte
void geraVizinhoBitFlip(Solucao *atual, Solucao* nova, int nC, int m) {
    copiaSolucao(nova, atual, nC);

    // escolhe indice aleatorio
    int r = geraNumEntre(0, nC - 1);
    int pos2;
    do {
        pos2 = geraNumEntre(0, nC - 1);
    }while (pos2 == r);

    //Inverte
    if (nova->sel[r] == 1) {
        nova->sel[r] = 0;
        nova->nSel--; // Ficou com m-1 (Inválida!)
    } else {
        nova->sel[r] = 1;
        nova->nSel++; // Ficou com m+1 (Inválida!)
    }
    if (nova->sel[pos2] == 1) {
        nova->sel[pos2] = 0;
        nova->nSel--; // Ficou com m-1 (Inválida!)
    } else {
        nova->sel[pos2] = 1;
        nova->nSel++; // Ficou com m+1 (Inválida!)
    }
}

// 1 -> erro, 0 -> tudo certo
int trepaColinas(Solucao *atual, float distancias[MAX_C][MAX_C],int m, int nC, int* numIter,float pen,int *countValido){
    Solucao* vizinho;

    vizinho = malloc(sizeof(Solucao));
    if(vizinho == NULL){
      printf("Erro ao alocar nova solucao.");
      return 1;
    }

    //Avalia solucao inicial
    atual->media = calculaPenalidade(atual,distancias,nC,m,pen);

    for(int i = 0; i < *numIter; i++){
        geraVizinhoBitFlip(atual,vizinho,nC, m);
        if (vizinho->nSel == m)
            (*countValido)++;
        vizinho->media = calculaPenalidade(vizinho,distancias,nC,m,pen);

        if(vizinho->media > atual->media){
            // Copia a solucao vizinho para a solucao atual, incluindo a media de distancias
            copiaSolucao(atual,vizinho,nC);
        }
    }

    free(vizinho);
    return 0;
}

int recristalizacao(Solucao *atual, float distancias[MAX_C][MAX_C], int m, int nC, int* numIter,
                       float pen, int* countValidos, float temperatura, float resfriamento, float temperaturaFinal){
    Solucao *vizinho, *melhorGlobal;

	vizinho = malloc(sizeof(Solucao));
    if(vizinho == NULL){
      printf("Erro ao alocar nova solucao.");
      return 1;
    }

    int k = *numIter;

    while(temperatura > temperaturaFinal){
        for(int i = 0; i < k; i++){
          	geraVizinhoBitFlip(atual,vizinho,nC, m);
            if (vizinho->nSel == m)
                (*countValidos)++;
            vizinho->media = calculaPenalidade(vizinho,distancias,nC,m,pen);

            //Avalia solucao inicial
    		atual->media = calculaPenalidade(atual,distancias,nC,m,pen);

            if(vizinho->media > atual->media){
                copiaSolucao(atual, vizinho, nC);
            } else {
                if(random_0_1() < exp((vizinho->media - atual->media) / temperatura)){
                    copiaSolucao(atual, vizinho, nC);
                }
            }
        	(*numIter)++;
        }
        temperatura*=resfriamento;
    }


    free(vizinho);
    return 0;
}

