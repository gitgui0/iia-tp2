#include "funcao.h"

//0->certo, 1->erro
int leInfo(int* nC, int* m, float distancias[MAX_C][MAX_C]) {
    char fich[100];

    printf("Insira o ficheiro a utilizar: ");
    scanf("%s",fich);

    printf("\nficherio. %s",fich);

    FILE *f = fopen(fich, "r");

    if (f == NULL) {
        printf("Esse ficheiro nao existe");
        return 1;
    }

    fscanf(f,"%d %d\n",nC,m);

    for(int i=0; i<*nC; i++)
        for(int j=0; j<*nC; j++)
            distancias[i][j] = 0.0;


    // Le as arestas (e1 e2 50.1...)
    char no1[10], no2[10];
    float dist;

    while (fscanf(f, "%s %s %f", no1, no2, &dist) == 3) {
        // 'no1' e string "e1", no1[1] e o '1' da string

        int id1 = atoi(&no1[1]) - 1;
        int id2 = atoi(&no2[1]) - 1;


        if (id1 >= 0 && id1 < *nC && id2 >= 0 && id2 < *nC) {
            distancias[id1][id2] = dist;
            distancias[id2][id1] = dist; // A matriz e simetrica
        }
    }

    fclose(f);

    // so para testar
    // mostraMatrizDistancias(distancias,*nC);

    return 0;
}


int main() {
    int nC,m;
    float distancias[MAX_C][MAX_C];

    if (leInfo(&nC,&m,distancias)!=0) {
        printf("\nErro o ler informacoes.");
        return 1;
    }

    // GERAR SOLUCAO

    Solucao sol;
    srand(time(NULL));
    geraSolucaoInicial(&sol, m, nC);

    printSol(&sol,nC);
    float q = devolveQualidade(&sol,distancias,nC,m);
    printf("\nQualidade: %.2f \n",q);


    return 0;
}