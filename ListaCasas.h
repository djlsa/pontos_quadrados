/*
 * ListaCasas.h
 * Estrutura que guarda uma lista de casas
 */

#ifndef LISTACASAS_H_
#define LISTACASAS_H_

#include <stdbool.h>
#include <stdlib.h>

#include "Casa.h"

/*
 * Estrutura tipo ListaCasas
 * casas: array de apontadores para as casas
 * capacidade: capacidade máxima da lista
 * indice: indice do próximo elemento a introduzir
 *
 */
typedef struct {
	Casa** casas;
	int capacidade;
	int indice;
} ListaCasas;

/*
 * "Construtor" da estrutura tipo ListaCasas
 * capacidade: capacidade máxima da lista
 *
 * Devolve um apontador para a estrutura criada
 */
ListaCasas* ListaCasas_new(int capacidade) {
	// Aloca espaço em memória para armazenar a estrutura
	ListaCasas* lista = malloc(sizeof(ListaCasas));

	// Aloca espaço em memória para o array
	lista->casas = malloc(sizeof(Casa*) * capacidade);

	// Inicialização de variáveis da estrutura
	lista->capacidade = capacidade;
	lista->indice = 0;

	return lista;
}

/*
 * "Destrutor" da estrutura tipo ListaCasas
 *
 * Liberta a memória utilizada por esta estrutura
 */
void ListaCasas_free(ListaCasas* lista) {
	// Liberta o array
	free(lista->casas);
	free(lista);
}

/*
 * Função que retorna o numero de elementos armazenados na lista
 * lista: apontador para a estrutura que representa a lista
 *
 * Devolve: numero de elementos
 */
int ListaCasas_length(ListaCasas* lista) {
	return lista->indice;
}

/*
 * Função que adiciona um elemento à lista
 * lista: apontador para a estrutura que representa a lista
 * casa: apontador para a casa a adicionar à lista
 *
 * Devolve: true ou false caso tenha sido adicionado ou não
 */
bool ListaCasas_add(ListaCasas* lista, Casa* casa) {
	// Se o indice for igual à capacidade significa que não
	// cabem mais elementos
	if(lista->indice < lista->capacidade) {
		// Adiciona e incrementa o indice
		lista->casas[lista->indice++] = casa;
		return true;
	}
	return false;
}

/*
 * Função que devolve um apontador para um elemento aleatório da lista
 * lista: apontador para a estrutura que representa a lista
 *
 * Devolve: apontador para elemento aleatório ou NULL caso a lista esteja vazia
 */
Casa* ListaCasas_get_random(ListaCasas* lista) {
	// Se a lista estiver vazia não é possível devolver uma casa
	if(lista->indice > 0)
		// Devolve um elemento aleatório
		return lista->casas[rand() % lista->indice];
	return NULL;
}

#endif
