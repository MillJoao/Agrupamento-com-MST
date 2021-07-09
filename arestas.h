#ifndef ARESTAS_H_
#define ARESTAS_H_
#include "ponto.h"

typedef struct arestas Arestas;


/* Inicializa o TAD com informação sobre todas as arestas entre os pontos passados
* Entradas: pontos - Vetor com todos os pontos (vértices) do grafo
* Saídas: Um ponteiro para o TAD criado, com o tamanho de todas as arestas já calculado
* Pós-condições: É alocado na memória o TAD Arestas e todas as informações sobre as arestas
*/
Arestas* criaArestas(Ponto** pontos);


/* Remove a menor aresta ainda presente em "arestas"
* Entradas: arestas - Ponteiro para o TAD com as arestas
* Saídas: -;
* Pós-condições: A aresta de menor tamanho é removida de arestas. 
*               Os conteudos de id_vertice_1 e 2 são alterados para os inteiros que representam os vértices componentes da aresta removida.
*/
void removeMenorAresta(Arestas* arestas, int *id_vertice_1, int *id_vertice_2);


/* Libera o TAD Arestas da memória
* Entradas: arestras - o ponteiro para o TAD a ser liberado
* Saídas: -;
* Pós-condições: A memória anteriormente alocada para arestas é liberada
*/
Arestas* destroiArestas(Arestas* arestas);







#endif