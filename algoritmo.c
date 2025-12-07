
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
        //geraVizinho(atual,vizinho,nC, m);
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
    atual->media = calculaPenalidade(atual,distancias,nC,m,pen);
    while(temperatura > temperaturaFinal){
        for(int i = 0; i < k; i++){
            //geraVizinho(atual,vizinho,nC, m);
          	geraVizinhoBitFlip(atual,vizinho,nC, m);
            if (vizinho->nSel == m)
                (*countValidos)++;
            vizinho->media = calculaPenalidade(vizinho,distancias,nC,m,pen);

            //Avalia solucao inicial

            if(vizinho->media > atual->media){
                // printf("Melhorou: %.2f -> %.2f\n", atual->media, vizinho->media);
                copiaSolucao(atual, vizinho, nC);
            } else {
                if(random_0_1() < exp((vizinho->media - atual->media) / temperatura)){
                    // printf("[ACEITOU PIOR] Temp: %.2f | Atual: %.2f | Vizinho: %.2f\n",temperatura, atual->media, vizinho->media);
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

//-----------------Evolutivo
void torneio(Solucao *pop, int popsize, int tsize, Solucao *pais) {
    int i, j, melhor, pos;

    for(i = 0; i < popsize; i++) {
        melhor = geraNumEntre(0, popsize-1);

        for(j = 1; j < tsize; j++) {
            pos = geraNumEntre(0, popsize-1); // Próximos candidatos
            if(pop[pos].media > pop[melhor].media) {
                melhor = pos;
            }
        }
        copiaSolucao(&pais[i], &pop[melhor], MAX_C); // MAX_C ou nC
    }
}

void roleta(Solucao *pop, int popsize, Solucao *pais) {
    float *fitness_ajustado = malloc(sizeof(float) * popsize);
    float soma_total = 0.0;

    // Encontrar o menor valor
    float min_fitness = pop[0].media;
    for (int i = 1; i < popsize; i++) {
        if (pop[i].media < min_fitness) {
            min_fitness = pop[i].media;
        }
    }

    // Calcular o Offset
    float offset = 0.0;
    if (min_fitness < 0) {
        // Só aplicamos a compensacao se houver valores negativos
        // O +1.0 garante que o pior indivíduo fica com valor 1 (probabilidade mínima) e não 0
        offset = -1 * min_fitness + 1.0;
    }

    // Preencher tabela de probabilidades ajustadas
    for (int i = 0; i < popsize; i++) {
        fitness_ajustado[i] = pop[i].media + offset;
        soma_total += fitness_ajustado[i];
    }

    // Se por azar a soma total for 0 (todos tinham fitness 0 e não houve offset),
    // evita a divisão por zero forçando soma = 1 (vai dar tudo zero, mas não crasha)
    if (soma_total == 0) soma_total = 1.0;

    // Rodar a Roleta
    for (int i = 0; i < popsize; i++) {
        float valor_sorteado = random_0_1() * soma_total;
        float soma_atual = 0.0;
        int selecionado = popsize - 1;

        for (int j = 0; j < popsize; j++) {
            soma_atual += fitness_ajustado[j];
            if (soma_atual >= valor_sorteado) {
                selecionado = j;
                break;
            }
        }
        copiaSolucao(&pais[i], &pop[selecionado], MAX_C);
    }

    free(fitness_ajustado);
}


void crossover_1ponto(Solucao *pais, int popsize, int nC, float pr, Solucao *filhos) {
    for (int i = 0; i < popsize; i += 2) {
        if (random_0_1() < pr) {
            int ponto = geraNumEntre(0, nC - 1);
            for (int j = 0; j < nC; j++) {
                if (j < ponto) {
                    filhos[i].sel[j] = pais[i].sel[j];
                    filhos[i+1].sel[j] = pais[i+1].sel[j];
                } else {
                    filhos[i].sel[j] = pais[i+1].sel[j];
                    filhos[i+1].sel[j] = pais[i].sel[j];
                }
            }
        } else {
            copiaSolucao(&filhos[i], &pais[i], nC);
            copiaSolucao(&filhos[i+1], &pais[i+1], nC);
        }
    }
}

void crossover_2pontos(Solucao *pais, int popsize, int nC, float pr, Solucao *filhos) {
    for (int i = 0; i < popsize; i += 2) {
        if (random_0_1() < pr) {
            int p1 = geraNumEntre(0, nC - 1);
            int p2 = geraNumEntre(0, nC - 1);
            if (p1 > p2) { int aux = p1; p1 = p2; p2 = aux; }

            for (int j = 0; j < nC; j++) {
                if (j < p1 || j >= p2) { // Mantém os extremos
                    filhos[i].sel[j] = pais[i].sel[j];
                    filhos[i+1].sel[j] = pais[i+1].sel[j];
                } else { // Troca o meio
                    filhos[i].sel[j] = pais[i+1].sel[j];
                    filhos[i+1].sel[j] = pais[i].sel[j];
                }
            }
        } else {
            copiaSolucao(&filhos[i], &pais[i], nC);
            copiaSolucao(&filhos[i+1], &pais[i+1], nC);
        }
    }
}

void crossover_uniforme(Solucao *pais, int popsize, int nC, float pr, Solucao *filhos) {
    for (int i = 0; i < popsize; i += 2) {
        if (random_0_1() < pr) {
            for (int j = 0; j < nC; j++) {
                if (random_0_1() < 0.5) { // 50% de troca
                    filhos[i].sel[j] = pais[i+1].sel[j];
                    filhos[i+1].sel[j] = pais[i].sel[j];
                } else {
                    filhos[i].sel[j] = pais[i].sel[j];
                    filhos[i+1].sel[j] = pais[i+1].sel[j];
                }
            }
        } else {
            copiaSolucao(&filhos[i], &pais[i], nC);
            copiaSolucao(&filhos[i+1], &pais[i+1], nC);
        }
    }
}


void mutacao_bitflip(Solucao *filhos, int popsize, int nC, float pm) {
    for (int i = 0; i < popsize; i++) {
        for (int j = 0; j < nC; j++) {
            if (random_0_1() < pm) {
                filhos[i].sel[j] = !filhos[i].sel[j];
            }
        }
    }
}

void mutacao_troca(Solucao *filhos, int popsize, int nC, float pm) {
    for (int i = 0; i < popsize; i++) {
        if (random_0_1() < pm) {
            int p1, p2;
            // Tenta encontrar um 0 e um 1 para trocar (tenta 100x para cada um para não encravar)
            int count = 0;
            do {
                p1 = geraNumEntre(0, nC-1);
                count++;
            } while (filhos[i].sel[p1] != 0 && count < 100);

            count = 0;
            do {
                p2 = geraNumEntre(0, nC-1);
                count++;
            } while (filhos[i].sel[p2] != 1 && count < 100);

            if (filhos[i].sel[p1] == 0 && filhos[i].sel[p2] == 1) {
                filhos[i].sel[p1] = 1;
                filhos[i].sel[p2] = 0;
            }
        }
    }
}

void geraSolucaoEvolutivo(Solucao *melhorGlobal, float distancias[MAX_C][MAX_C], int nC, int m, float pen, Evolutivo ev, Solucao* pop, Solucao* pais, Solucao* filhos) {
    
    Solucao best_run;
    best_run.media = -1.0;

    for (int i = 0; i < ev.popsize; i++) {
        geraSolucaoInicial(&pop[i], m, nC);
    }
    
    avaliaPopulacao(pop, ev.popsize, distancias, nC, m, ev.tipoReparacao);

    // Encontra o melhor inicial
    for (int i = 0; i < ev.popsize; i++) {
        if (pop[i].media > best_run.media) {
            if(ev.tipoReparacao == 0 && pop[i].nSel != m)
                continue;
            copiaSolucao(&best_run, &pop[i], nC);
        }
    }

    // Ciclo das Gerações
    for (int g = 0; g < ev.numGenerations; g++) {
        switch(ev.tipoRecombinacao) {
            case 2: roleta(pop, ev.popsize, pais); break;
            default: torneio(pop, ev.popsize, ev.tsize, pais); break;
        }


        // Operadores Geneticos


        // Crossover
        switch(ev.tipoRecombinacao) {
            case 1: crossover_1ponto(pais, ev.popsize, nC, ev.prc, filhos); break;
            case 2: crossover_2pontos(pais, ev.popsize, nC, ev.prc, filhos); break;
            default: crossover_uniforme(pais, ev.popsize, nC, ev.prc, filhos); break;
        }

        // Mutacao
        // 1 - troca ou 2 -bitflip
        if (ev.tipoMutacao == 2)
            mutacao_troca(filhos, ev.popsize, nC, ev.pmt);
        else
            mutacao_bitflip(filhos, ev.popsize, nC, ev.pmt);

        // Reparacao e avaliacao dos filhos
        avaliaPopulacao(filhos, ev.popsize, distancias, nC, m, ev.tipoReparacao);

        // Atualiza melhor da run com a nova geração
        for (int i = 0; i < ev.popsize; i++) {
            if (filhos[i].media > best_run.media) {
                // se for penalizacao nao copia
                if(ev.tipoReparacao == 0 && filhos[i].nSel != m)
                    continue;
                copiaSolucao(&best_run, &filhos[i], nC);
            }
        }

        // Substituição (Geracional pura: filhos substituem pais)
        for(int i=0; i<ev.popsize; i++){
            copiaSolucao(&pop[i], &filhos[i], nC);
        }
    }

    // Copia para melhorGlobal para mostrar no main
    copiaSolucao(melhorGlobal, &best_run, nC);
}


void geraSolucaoHibrido(Solucao *melhorGlobal, float distancias[MAX_C][MAX_C], int nC, int m, float pen,
                        Evolutivo ev, Solucao* pop, Solucao* pais, Solucao* filhos, int abordagem, int algoritmoEscolhido, float temperatura, float arrefecimento, float temperaturaFinal, int numIter){

    Solucao best_run;
    best_run.media = -1.0;

    for (int i = 0; i < ev.popsize; i++) {
        geraSolucaoInicial(&pop[i], m, nC);
    }

    avaliaPopulacao(pop, ev.popsize, distancias, nC, m, ev.tipoReparacao);

    if (abordagem==1) {
        if (algoritmoEscolhido == 1) {
            for (int i = 0; i < ev.popsize; i++) {
                int num = numIter;
                int temp;
                trepaColinas(&pop[i], distancias, m, nC, &num, pen, &temp);
            }
        }
        if (algoritmoEscolhido == 2) {
            for (int i = 0; i < ev.popsize; i++) {
                int num = numIter;
                int temp;
                recristalizacao(&pop[i], distancias, m, nC, &num, pen, &temp, temperatura, arrefecimento, temperaturaFinal);
            }
        }
    }

    // Encontra o melhor inicial
    for (int i = 0; i < ev.popsize; i++) {
        if (pop[i].media > best_run.media) {
            if(ev.tipoReparacao == 0 && pop[i].nSel != m)
                continue;
            copiaSolucao(&best_run, &pop[i], nC);
        }
    }

    // Ciclo das Gerações
    for (int g = 0; g < ev.numGenerations; g++) {
        switch(ev.tipoRecombinacao) {
            case 2: roleta(pop, ev.popsize, pais); break;
            default: torneio(pop, ev.popsize, ev.tsize, pais); break;
        }


        // Operadores Geneticos


        // Crossover
        switch(ev.tipoRecombinacao) {
            case 1: crossover_1ponto(pais, ev.popsize, nC, ev.prc, filhos); break;
            case 2: crossover_2pontos(pais, ev.popsize, nC, ev.prc, filhos); break;
            default: crossover_uniforme(pais, ev.popsize, nC, ev.prc, filhos); break;
        }

        // Mutacao
        // 1 - troca ou 2 -bitflip
        if (ev.tipoMutacao == 2)
            mutacao_troca(filhos, ev.popsize, nC, ev.pmt);
        else
            mutacao_bitflip(filhos, ev.popsize, nC, ev.pmt);

        // Reparacao e avaliacao dos filhos
        avaliaPopulacao(filhos, ev.popsize, distancias, nC, m, ev.tipoReparacao);

        // Atualiza melhor da run com a nova geração
        for (int i = 0; i < ev.popsize; i++) {
            if (filhos[i].media > best_run.media) {
                // se for penalizacao nao copia
                if(ev.tipoReparacao == 0 && filhos[i].nSel != m)
                    continue;
                copiaSolucao(&best_run, &filhos[i], nC);
            }
        }

        // Substituição (Geracional pura: filhos substituem pais)
        for(int i=0; i<ev.popsize; i++){
            copiaSolucao(&pop[i], &filhos[i], nC);
        }
    }

    if (abordagem==2) {
        if (algoritmoEscolhido == 1) {
            for (int i = 0; i < ev.popsize; i++) {
                int num = numIter;
                int temp; // countValido
                trepaColinas(&pop[i], distancias, m, nC, &num, pen, &temp);
            }
        }
        if (algoritmoEscolhido == 2) {
            for (int i = 0; i < ev.popsize; i++) {
                int num = numIter;
                int temp; // countValidos
                recristalizacao(&pop[i], distancias, m, nC, &num, pen, &temp, temperatura, arrefecimento, temperaturaFinal);
            }
        }
    }

    // Calcula o melhor outra vez
    for (int i = 0; i < ev.popsize; i++) {
        if (pop[i].media > best_run.media) {
            if(ev.tipoReparacao == 0 && pop[i].nSel != m)
                continue;
            copiaSolucao(&best_run, &pop[i], nC);
        }
    }

    // Copia para melhorGlobal para mostrar no main
    copiaSolucao(melhorGlobal, &best_run, nC);
}