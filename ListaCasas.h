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
 * capacidade: capacidade m�xima da lista
 * indice: indice do pr�ximo elemento a introduzir
 *
 */
typedef struct {
	Casa** casas;
	int capacidade;
	int indice;
} ListaCasas;

/*
 * "Construtor" da estrutura tipo ListaCasas
 * capacidade: capacidade m�xima da lista
 *
 * Devolve um apontador para a estrutura criada
 */
ListaCasas* ListaCasas_new(int capacidade) {
	// Aloca espa�o em mem�ria para armazenar a estrutura
	ListaCasas* lista = malloc(sizeof(ListaCasas));

	// Aloca espa�o em mem�ria para o array
	lista->casas = malloc(sizeof(Casa*) * capacidade);

	// Inicializa��o de vari�veis da estrutura
	lista->capacidade = capacidade;
	lista->indice = 0;

	return lista;
}

/*
 * "Destrutor" da estrutura tipo ListaCasas
 *
 * Liberta a mem�ria utilizada por esta estrutura
 */
void ListaCasas_free(ListaCasas* lista) {
	// Liberta o array
	free(lista->casas);
	free(lista);
}

/*
 * Fun��o que retorna o numero de elementos armazenados na lista
 * lista: apontador para a estrutura que representa a lista
 *
 * Devolve: numero de elementos
 */
int ListaCasas_length(ListaCasas* lista) {
	return lista->indice;
}

/*
 * Fun��o que adiciona um elemento � lista
 * lista: apontador para a estrutura que representa a lista
 * casa: apontador para a casa a adicionar � lista
 *
 * Devolve: true ou false caso tenha sido adicionado ou n�o
 */
bool ListaCasas_add(ListaCasas* lista, Casa* casa) {
	// Se o indice for igual � capacidade significa que n�o
	// cabem mais elementos
	if(lista->indice < lista->capacidade) {
		// Adiciona e incrementa o indice
		lista->casas[lista->indice++] = casa;
		return true;
	}
	return false;
}

/*
 * Fun��o que devolve um apontador para um elemento aleat�rio da lista
 * lista: apontador para a estrutura que representa a lista
 *
 * Devolve: apontador para elemento aleat�rio ou NULL caso a lista esteja vazia
 */
Casa* ListaCasas_get_random(ListaCasas* lista) {
	// Se a lista estiver vazia n�o � poss�vel devolver uma casa
	if(lista->indice > 0)
		// Devolve um elemento aleat�rio
		return lista->casas[rand() % lista->indice];
	return NULL;
}

#endif
