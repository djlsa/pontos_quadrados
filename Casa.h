/*
 * Casa.h
 * Representa uma casa na grelha de jogo, ou seja, cada quadrado poss�vel de ser
 * preenchido
 */

#ifndef CASA_H_
#define CASA_H_

#include <stdbool.h>

#include "Jogador.h"

/*
 * Estrutura tipo Casa
 * linha, coluna: coordenadas da casa dentro da grelha
 * cima, esquerda, baixo, direita: indica se as paredes est�o ou n�o preenchidas
 * lados: contagem de quantos lados est�o preenchidos
 * jogador: caractere que representa o jogador que completou o quadrado
 */
typedef struct {
	int linha, coluna;
	bool cima, esquerda, baixo, direita;
	int lados;
	char jogador;
} Casa;

// Caractere que indica que o quadrado ainda n�o est� preenchido (vazio)
const char CASA_LIVRE = ' ';

/*
 * Enumera��o dos lados poss�veis para efetuar uma jogada
 */
typedef enum {
	CIMA, BAIXO, ESQUERDA, DIREITA
} ELado;

/*
 * Enumera��o dos diferentes resultados poss�veis para uma jogada:
 * JOGADA_CAPTURA: quando se completa um quadrado
 * JOGADA_VALIDA: quando � preenchido um dos lados do quadrado
 * JOGADA_INVALIDA: quando n�o � poss�vel efetuar a jogada
 */
typedef enum {
	JOGADA_VALIDA,
	JOGADA_INVALIDA,
	JOGADA_CAPTURA
} EResultadoJogada;

/*
 * "Construtor" da estrutura tipo Casa
 * linha, coluna: coordenadas da casa dentro da grelha
 *
 * Devolve um apontador para a estrutura criada
 */
Casa* Casa_new(int linha, int coluna) {
	// Aloca espa�o em mem�ria para armazenar a estrutura
	Casa* casa = malloc(sizeof(Casa));

	// Inicializa��o das vari�veis da estrutura
	casa->linha = linha;
	casa->coluna = coluna;
	casa->cima = false;
	casa->esquerda = false;
	casa->baixo = false;
	casa->direita = false;
	casa->lados = 0;
	casa->jogador = CASA_LIVRE;

	return casa;
}

/*
 * Fun��o que efetua uma jogada numa determinada casa
 * casa: apontador para a casa onde ser� efetuada a jogada
 * lado: lado a ser preenchido
 * jogador: apontador para a estrutura que representa o jogador
 *
 * Devolve o resultado da jogada
 */
EResultadoJogada Casa_jogar(Casa* casa, ELado lado, Jogador* jogador) {
	// Se a casa j� tiver sido capturada, a jogada � inv�lida
	if(casa->jogador != CASA_LIVRE)
		return JOGADA_INVALIDA;

	// Lado a preencher
	switch(lado) {
		case CIMA:
			// Se o lado j� estiver preenchido...
			if(casa->cima)
				// ...ent�o a jogada � inv�lida
				return JOGADA_INVALIDA;
			// Caso contr�rio (n�o havendo return), preenche-se a casa
			casa->cima = true;
			break;
		// L�gica identica para os restantes lados
		case ESQUERDA:
			if(casa->esquerda)
				return JOGADA_INVALIDA;
			casa->esquerda = true;
			break;
		case BAIXO:
			if(casa->baixo)
				return JOGADA_INVALIDA;
			casa->baixo = true;
			break;
		case DIREITA:
			if(casa->direita)
				return JOGADA_INVALIDA;
			casa->direita = true;
			break;
	}
	// Incrementa-se a contagem de lados preenchidos
	casa->lados++;
	// Se todos os lados estiverem preenchidos...
	if(casa->cima && casa->esquerda && casa->baixo && casa->direita) {
		// Regista-se o simbolo do jogador que preencheu o quadrado
		casa->jogador = jogador->simbolo;
		// Incrementa-se o numero de casas capturadas pelo jogador
		jogador->casas++;
		// Retorna-se uma indica��o de ter ocorrido uma captura para se dar a
		// possibilidade de jogar novamente
		return JOGADA_CAPTURA;
	}
	// Chegando a este ponto, � porque ocorreu uma jogada v�lida
	return JOGADA_VALIDA;
}

#endif
