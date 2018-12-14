/*
 * Jogador.h
 * Representa um jogador
 */

#ifndef JOGADOR_H_
#define JOGADOR_H_

#include <stdbool.h>

/*
 * Enumeração dos tipos de jogador; humano e artificial
 */
typedef enum {
	JOGADOR_HUMANO,
	JOGADOR_ARTIFICIAL
} ETipoJogador;

/*
 * Estrutura tipo Jogador
 * tipo: tipo do jogador representado
 * simbolo: caractere que representa o jogador quando completa um quadrado
 * casas: contagem de quantos quadrados conseguiu completar
 */
typedef struct {
	ETipoJogador tipo;
	char simbolo;
	int casas;
} Jogador;

/*
 * "Construtor" da estrutura tipo Casa
 * tipo: tipo do jogador
 * simbolo: caractere que representa o jogador
 *
 * Devolve um apontador para a estrutura criada
 */
Jogador* Jogador_new(ETipoJogador tipo, char simbolo) {
	// Aloca espaço em memória para armazenar a estrutura
	Jogador* jogador = malloc(sizeof(Jogador));

	// Inicialização das variáveis da estrutura
	jogador->tipo = tipo;
	jogador->simbolo = simbolo;
	jogador->casas = 0;

	return jogador;
}

#endif
