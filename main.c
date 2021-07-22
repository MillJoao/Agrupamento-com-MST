#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ponto.h"
#include "arestas.h"
#include "grupos.h"

void merge(Ponto** pontos, int lo, int m, int hi) {

    int i, j, k;
    int n1 = m - lo + 1;
    int n2 = hi - m;

    // Cria vetores de ponteiros para ponto auxiliares, um para cada metade do vetor

    Ponto** L = malloc(sizeof(Ponto*) * n1);
    Ponto** R = malloc(sizeof(Ponto*) * n2);

    // Copia os dados de cada metade do vetor pontos para o auxiliar correspondente
    for (i = 0; i < n1; i++) L[i] = pontos[lo + i];
    for (j = 0; j < n2; j++) R[j] = pontos[m + 1+ j];

    // Merge os vetores auxiliares
    i = 0;
    j = 0;
    k = lo;
    while (i < n1 && j < n2) {
        if (strcmp(retornaNome(L[i]), retornaNome(R[j])) <= 0) {
            pontos[k] = L[i];
            i++;
        }
        else {
            pontos[k] = R[j];
            j++;
        }
        k++;
    }
    // Copia os elementos restantes de L
    while (i < n1) {
        pontos[k] = L[i];
        i++;
        k++;
    }
    // Copia os elementos restantes de R
    while (j < n2) {
        pontos[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}


void mergeSort(Ponto** pontos, int lo, int hi) {

    /*Aplicação do algoritmo mergeSort recursivo visto em aula*/
    if (lo < hi) {
        int m = lo+(hi-lo)/2;
        mergeSort(pontos, lo, m);
        mergeSort(pontos, m+1, hi);
        merge(pontos, lo, m, hi);
    }
}

void imprimeSaida(Grupos* grupos, int k, char* saida){
    
    FILE *file = fopen(saida, "w");
    Ponto ***pGrupos; // k vetores de ponteiros para ponto, cada k representa um grupo
    Ponto **Copia = malloc(sizeof(Ponto*) * retornaNumeroPontos(grupos)); // Vetor de ponteiros para ponto que conterá a cópia do endereço de todos os pontos
    int ID, i, j=0, index, tamanho;



    fclose(file);
    /*Preenche o vetor Copia com os endereços de todos os pontos*/
    for (i = 0; i<retornaNumeroPontos(grupos); i++){
        Copia[i] = retornaPontoPorIndex(grupos, i);

        /*faz com que cada ponto tenha o ID da raíz*/
        while (retornaID(retornaPontoPorIndex(grupos, retornaID(Copia[i]))) != retornaIndex(retornaPontoPorIndex(grupos, retornaID(Copia[i])))){
            mudaID(Copia[i], retornaID(retornaPontoPorIndex(grupos, retornaID(Copia[i]))));
        }
    }

    /*Obtém-se o vetor contendo os tamanhos dos grupos*/
    int* tamanhoGrupos = retornaTamanhoGrupos(grupos);

    /*Aloca espaço para os k grupos*/
    pGrupos = malloc(sizeof(Ponto**)*k);

    for (i = 0; i<k; i++){
        index = 0;
        /*Os endereços do vetor Copia são removidos quando já analisados*/
        while(Copia[j] == NULL) j++;

        /*Obtém-se o ID do próximo grupo a ser analisado*/
        ID = retornaID(retornaPontoPorIndex(grupos,j));

        /*Obtém-se o tamanho do grupo para alocação de memória*/
        tamanho = tamanhoGrupos[ID];
        int aux_tamanho = tamanho;
        pGrupos[i] = malloc(sizeof(Ponto*) * tamanho);

        /*Preenche o vetor alocado com os pontos de mesma ID*/
        for(int l=0; l<retornaNumeroPontos(grupos); l++){
            if (ID == retornaID(retornaPontoPorIndex(grupos,l))){
                aux_tamanho--;
                pGrupos[i][index] = retornaPontoPorIndex(grupos,l);
                index++;
                Copia[l] = NULL;

                
                if (aux_tamanho < 1){

                    /*Apos preencher o vetor com pontos de mesma ID, ordena o mesmo e imprime os grupos na saida*/
                    mergeSort(pGrupos[i], 0, tamanho-1);

                    file = fopen(saida, "a");

                    for(j = 0; j<tamanho; j++){
                        fprintf(file, "%s",retornaNome(pGrupos[i][j]));
                        if (j<tamanho-1) fprintf(file, ",");
                    }
                    fprintf(file, "\n");

                    fclose(file);
                    j=0;
                    break;
                } 
            }
        }
    }

    free(Copia);
    
    for (i = 0; i<k; i++){
        free(pGrupos[i]);
    }

    free(pGrupos);
}

Ponto** leEntrada(char* entrada, int* count){

    FILE* file = fopen(entrada, "r");
    if (file == NULL){
        printf("Arquivo de entrada não encontrado\n");
        return NULL;
    }

    size_t buffer = 100; //unsigned int contendo o tamanho da string que recebera as linhas da entrada (tipo padrao do getiline)
    char* linha = malloc(sizeof(char)*buffer);
    char* token = NULL;
    char* nome = NULL;
    long double X, Y;

    /*Conta o numero de linhas contidas no arquivo de entrada, necessario para alocacao de espaco para o vetor contendo os ponteiros do tipo ponto*/
    while (getline (&linha, &buffer, file) != -1) (*count)++; 

    Ponto** pontos = malloc(sizeof(Ponto*) * (*count));

    /*Reseta o ponteiro de leitura para o incio do arquivo de entrada, para agora obter os dados*/
    fseek(file, 0, SEEK_SET);

    for (int i = 0; i < *count; i++){

        getline (&linha, &buffer, file);

        /*Obtem-se cada elemento que eh separado por virgula*/
        token = strtok(linha, ",");       

        nome = strdup(token);

        token = strtok(NULL, ",");
        X = atof(token);

        token = strtok(NULL, ",\n");
        Y = atof(token);

        pontos[i] = criaPonto(nome, X, Y, i);
        if(nome) free(nome);
    }

    if(linha) free(linha);
    fclose(file);
    
    return pontos;
}

/*algorithm Kruskal(G) is
    F:= ∅
    for each v ∈ G.V do
        MAKE-SET(v)
    for each (u, v) in G.E ordered by weight(u, v), increasing do
        if FIND-SET(u) ≠ FIND-SET(v) then
            F:= F ∪ {(u, v)} ∪ {(v, u)}
            UNION(FIND-SET(u), FIND-SET(v))
    return F*/

void Kruskal (Grupos* grupos, int k){
    Arestas* arestas = criaArestas(grupos);
    int numeroArvores = retornaNumeroPontos(grupos), idVertice1, idVertice2;
    
    while(numeroArvores > k){
        removeMenorAresta(arestas, &idVertice1, &idVertice2);
        
        if (UF_Find(grupos, idVertice1) != UF_Find(grupos, idVertice2)){
            UF_Union(grupos, retornaPontoPorIndex(grupos, idVertice1), retornaPontoPorIndex(grupos, idVertice2));
            
            numeroArvores--;
        }
    }

    destroiArestas(arestas);
}

int main(int argc, char** argv){

    char *entrada, *saida;
    entrada = argv[1];
    saida = argv[3];
    
    int k = atoi(argv[2]);

    int i, count = 0;
    Ponto** pontos = leEntrada(entrada, &count);
    Grupos* grupos = inicializaGrupos(pontos, count);

    printf("------\nTodos os pontos:\n");
    for(i = 0; i < count; i++){
        imprimePonto(retornaPontoPorIndex(grupos, i));
    }

    Kruskal(grupos, k);
    
    ////////testes/////////
    //printf("%d\n", retornaNumeroPontos(grupos));
    //printf("Distancia entre os dois primeiros pontos eh: %Lf\n\n", distanciaEuclidiana(retornaPontoPorIndex(grupos, 1), retornaPontoPorIndex(grupos, 0)));


    printf("------\nTodos os pontos:\n");
    for(i = 0; i < count; i++){
        imprimePonto(retornaPontoPorIndex(grupos, i));
    }

    printf("\n\n");
    imprimeSaida(grupos, k, saida);

    //Liberar a memória
    destroiGrupos(grupos);
    return 0;
}