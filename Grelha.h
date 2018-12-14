/*
 * Grelha.h
 * Representa a grelha de jogo, com todas as casas
 */

#ifndef GRELHA_H_
#define GRELHA_H_

#include <stdbool.h>

#include "Casa.h"
#include "Coordenada.h"

/*
 * Estrutura tipo Grelha
 * linhas, colunas: numero de linhas e colunas da grelha
 * jogadas_restantes: numero de jogadas ainda poss�veis de efetuar
 * casas: array bidimensional de apontadores para as casas
 */
typedef struct {
	int linhas, colunas;
	int jogadas_restantes;
	Casa*** casas;
} Grelha;

/*
 * "Construtor" da estrutura tipo Grelha
 * linhas, colunas: numero de linhas e colunas da grelha
 *
 * Devolve um apontador para a estrutura criada
 */
Grelha* Grelha_new(linhas, colunas) {
	// Aloca espa�o em mem�ria para armazenar a estrutura
	Grelha* grelha = malloc(sizeof(Grelha));

	// Inicializa��o das vari�veis da estrutura
	grelha->linhas = linhas;
	grelha->colunas = colunas;

	// C�lculo do numero de jogadas restantes:
	// lados esquerdo e superior: 2xlinhas*colunas
	// lado direito e inferior das casas nas extremidades respetivas
	grelha->jogadas_restantes = 2 * linhas * colunas + linhas + colunas;

	// Aloca espa�o em mem�ria para a primeira dimens�o do array
	grelha->casas = malloc(sizeof(Casa**) * linhas);

	int linha, coluna;
	// Percorre as linhas...
	for(linha = 0; linha < linhas; linha++) {
		// Por cada linha, aloca espa�o para a segunda dimens�o do array
		grelha->casas[linha] = malloc(sizeof(Casa*) * colunas);
		// Percorre as colunas...
		for(coluna = 0; coluna < colunas; coluna++)
			// Guarda um apontador para cada casa
			grelha->casas[linha][coluna] = Casa_new(linha, coluna);
	}

	return grelha;
}

/*
 * "Destrutor" da estrutura tipo Grelha
 * grelha: apontador para a estrutura que representa a grelha
 *
 * Liberta a mem�ria utilizada por esta estrutura
 */
void Grelha_free(Grelha* grelha) {
	int linha, coluna;
	for(linha = 0; linha < grelha->linhas; linha++) {
		for(coluna = 0; coluna < grelha->colunas; coluna++)
			// Liberta a mem�ria de cada casa
			free(grelha->casas[linha][coluna]);
		// Liberta a linha (array de colunas)
		free(grelha->casas[linha]);
	}
	// Liberta o array (array de linhas)
	free(grelha->casas);
	free(grelha);
}

/*
 * Fun��o que efetua uma jogada numa determinada coordenada
 * grelha: apontador para a grelha onde ser� efetuada a jogada
 * linha, coluna: coordenadas
 * lado: lado do quadrado a ser preenchido
 * jogador: apontador para a estrutura que representa o jogador
 *
 * Devolve o resultado da jogada
 */
EResultadoJogada Grelha_jogar_int(Grelha* grelha, int linha, int coluna,
												ELado lado, Jogador* jogador) {
	// Auxiliares para facilitar a escrita
	int linhas = grelha->linhas, colunas = grelha->colunas;
	// Testa se a jogada � feita fora dos limites da grelha...
	if(linha < 0 || linha >= linhas || coluna < 0 || coluna >= colunas)
		return JOGADA_INVALIDA;
	// Auxiliar para facilitar a escrita
	Casa*** casas = grelha->casas;
	// Resultados: 1� para a jogada na casa onde � efetuada a jogada diretamente
	// e 2� para a jogada na casa adjacente
	EResultadoJogada resultado1, resultado2;
	// Por omiss�o, considera-se a jogada inv�lida e faz-se a valida��o
	// em seguida
	resultado1 = resultado2 = JOGADA_INVALIDA;
	if(lado == CIMA || lado == BAIXO) {
	// Dire��o horizontal
		resultado1 = Casa_jogar(casas[linha][coluna], lado, jogador);

		if(lado == CIMA && linha > 0)
		// Caso n�o seja a primeira linha, preencher a parede superior �
		// equivalente a preencher tamb�m a inferior da casa imediatamente acima
			resultado2 = Casa_jogar(casas[linha - 1][coluna], BAIXO, jogador);
		else if(lado == BAIXO && linha < linhas - 1)
		// Inversamente, caso n�o seja a �ltima, preencher a inferior tamb�m
		// � equivalente a preencher a superior da casa de baixo
			resultado2 = Casa_jogar(casas[linha + 1][coluna], CIMA, jogador);
	} else if(lado == ESQUERDA || lado == DIREITA) {
	// Dire��o vertical
		resultado1 = Casa_jogar(casas[linha][coluna], lado, jogador);

		// Aplica-se a mesma l�gica da dire��o horizontal mas sobre as colunas
		if(lado == ESQUERDA && coluna > 0)
			resultado2 = Casa_jogar(casas[linha][coluna - 1], DIREITA, jogador);
		else if(lado == DIREITA && coluna< colunas - 1)
			resultado2 = Casa_jogar(casas[linha][coluna + 1],ESQUERDA, jogador);
	}
	// Se a primeira jogada for v�lida (com ou sem captura)...
	if(resultado1 != JOGADA_INVALIDA) {
		// Subtrai-se o numero de jogadas restantes
		grelha->jogadas_restantes--;
		// Verifica-se se ocorreu captura na casa adjacente
		if(resultado2 == JOGADA_CAPTURA)
			return resultado2;
	}
	return resultado1;
}

/*
 * Fun��o que efetua uma jogada numa determinada coordenada
 * grelha: apontador para a grelha onde ser� efetuada a jogada
 * coordenada: apontador para uma estrutura que representa uma coordenada
 * jogador: apontador para a estrutura que representa o jogador
 *
 * Devolve o resultado da jogada
 */
EResultadoJogada Grelha_jogar_coordenada(Grelha* grelha, Coordenada* coordenada,
															Jogador* jogador) {
	// Auxiliares para facilitar a escrita
	int
		linha = coordenada->linha, linhas = grelha->linhas,
		coluna = coordenada->coluna, colunas = grelha->colunas;
	EDirecao direcao = coordenada->direcao;

	ELado lado;
	// Por omiss�o, a parede a preencher � a superior ou a esquerda, consoante
	// a dire��o seja horizontal ou vertical respetivamente. No caso de a
	// coordenada representar uma casa na extremidade inferior ou direita da
	// grelha, o lado a ser preenchido ser� o oposto.
	if(direcao == HORIZONTAL) {
		lado = linha < linhas ? CIMA : BAIXO;
		if(lado == BAIXO)
			// Corre��o para o indice v�lido
			coordenada->linha--;
	} else if(direcao == VERTICAL) {
		lado = coluna < colunas ? ESQUERDA : DIREITA;
		if(lado == DIREITA)
			coordenada->coluna--;
	}

	return Grelha_jogar_int(grelha, coordenada->linha, coordenada->coluna, lado,
																	jogador);
}

/*
 * Fun��o que mostra a grelha em output
 * grelha: apontador para a grelha onde ser� efetuada a jogada
 *
 */
void Grelha_print(Grelha* grelha) {
	// Auxiliares para facilitar a escrita
	int
		linha, linha_print, linhas = grelha->linhas, linhas_print = linhas * 2,
		coluna, colunas = grelha->colunas;
	Casa*** casas = grelha->casas;

	// Inicia-se o ciclo a -1 para escrever o numero das colunas
	for(linha_print = -1; linha_print <= linhas_print; linha_print++) {
		// Regista-se se a linha atual de output � par
		bool linha_par = linha_print % 2 == 0;
		// Linha real para utilizar como indice
		linha = linha_print / 2;
		for(coluna = 0; coluna <= colunas; coluna++) {
			// Numeros das colunas na primeira linha
			if(linha_print == -1) {
				printf("  %d", coluna);
				if(coluna < 10)
					// Para n�o desalinhar
					printf(" ");
			} else {
			// Letras � esquerda e grelha de jogo � direita
				// Na primeira coluna...
				if(coluna == 0) {
					// Se for zero ou par...
					if(linha_par)
						// Escreve-se a letra da linha
						printf("%c ", linha + 'A');
					else
						printf("  ");
				}
				// Grelha de jogo
				// Se a coluna for um indice v�lido...
				if(coluna < colunas) {
					if(!linha_par) {
						// Se a linha for impar desenham-se os tra�os
						// verticais e os simbolos dos jogadores

						// Desenha-se uma parede ou um espa�o
						printf("%c",
							casas[linha][coluna]->esquerda ? '|' : ' ');
						// Coloca-se ou caractere do jogador que capturou
						// a casa
						printf(" %c ", casas[linha][coluna]->jogador);
						// Caso seja a �ltima coluna de casas, desenha-se
						// uma parede no lado direito ou um espa�o
						if(coluna == colunas - 1)
							printf("%c",
								casas[linha][coluna]->direita ? '|' : ' ');
					} else {
						// Nas linhas pares desenham-se os tra�os horizontais
						// e os pontos da grelha

						printf("+");

						// Em todas as linhas menos na �ltima, considera-se a
						// parede de cima, na �ltima a de baixo
						if((linha < linhas && casas[linha][coluna]->cima) ||
						(linha == linhas && casas[linha - 1][coluna]->baixo) )
							printf("---");
						else
							printf("   ");

						if(coluna == colunas - 1)
							printf("+");
					}
				}
			}
		}
		printf("\n");
	}
}

#endif
