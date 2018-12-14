/*
 * Jogo.h
 * Representa um jogo com jogadores humanos e/ou artificiais
 */

#ifndef JOGO_H_
#define JOGO_H_

#include <stdbool.h>
#include <time.h>

#include "Grelha.h"
#include "ListaCasas.h"

// Número máximo de jogadores por jogo
const int JOGO_MAXIMO_JOGADORES = 6;
// Simbolos atribuidos aos jogadores
const char* JOGO_SIMBOLOS = "OX+#@$";

/*
 * Estrutura tipo Jogo
 * grelha: grelha de jogo
 * jogadores: array de apontadores para os jogadores
 * numero_jogadores: numero de jogadores
 * jogador_atual: apontador para o jogador que faz a próxima jogada
 * indice_jogador_atual: indice do jogador atual, no array de jogadores
 * estado: estado atual do jogo; a decorrer ou terminado
 */
typedef struct {
	Grelha* grelha;
	Jogador** jogadores;
	int numero_jogadores;
	Jogador* jogador_atual;
	int indice_jogador_atual;
	enum { JOGO_DECORRER, JOGO_TERMINADO } estado;
} Jogo;

/*
 * "Construtor" da estrutura tipo Jogo
 * linhas, colunas: numero de linhas e colunas da grelha
 * total_jogadores: numero total de jogadores
 * jogadores_artificiais: quantos jogadores são artificiais
 *
 * Devolve um apontador para a estrutura criada
 */
Jogo* Jogo_new(int linhas, int colunas, int total_jogadores,
													int jogadores_artificiais) {
	// Aloca espaço em memória para armazenar a estrutura
	Jogo* jogo = malloc(sizeof(Jogo));

	// Inicialização da grelha
	jogo->grelha = Grelha_new(linhas, colunas);

	// Restringe o numero de jogadores
	if(total_jogadores > JOGO_MAXIMO_JOGADORES)
		total_jogadores = JOGO_MAXIMO_JOGADORES;

	// Inicialização do array de apontadores para os jogadores
	jogo->jogadores = malloc(sizeof(Jogador*) * total_jogadores);

	// Se existirem jogadores artificiais, fazer "seed" ao gerador de
	// numeros aleatorios
	if(jogadores_artificiais > 0)
		srand(time(NULL));

	// Colocar os jogadores artificiais no fim da lista de jogadores
	int indice_ai = total_jogadores - jogadores_artificiais;
	int i;
	for(i = 0; i < total_jogadores; i++) {
		jogo->jogadores[i] = Jogador_new(
						(i < indice_ai ? JOGADOR_HUMANO : JOGADOR_ARTIFICIAL),
						JOGO_SIMBOLOS[i]
					);
	}

	// Inicialização de variáveis da estrutura
	jogo->numero_jogadores = total_jogadores;
	jogo->jogador_atual = total_jogadores > 0 ? jogo->jogadores[0] : NULL;
	jogo->indice_jogador_atual = jogo->jogador_atual != NULL ? 0 : -1;
	jogo->estado = JOGO_DECORRER;

	return jogo;
}

/*
 * "Destrutor" da estrutura tipo Jogo
  * jogo: apontador para a estrutura que representa o jogo
 *
 * Liberta a memória utilizada por esta estrutura
 */
void Jogo_free(Jogo* jogo) {
	// Liberta a grelha
	Grelha_free(jogo->grelha);
	int i;
	for(i = 0; i < jogo->numero_jogadores; i++)
		// Liberta cada jogador
		free(jogo->jogadores[i]);
	// Liberta o array de jogadores
	free(jogo->jogadores);
	free(jogo);
}

/*
 * Função que muda para o próximo jogador
 * jogo: apontador para a estrutura que representa o jogo
 *
 */
void _Jogo_trocar_jogador(Jogo* jogo) {
	if(++jogo->indice_jogador_atual == jogo->numero_jogadores)
		jogo->indice_jogador_atual = 0;
	jogo->jogador_atual = jogo->jogadores[jogo->indice_jogador_atual];
}

/*
 * Função que verifica o numero de jogadas restantes e atualiza o estado do jogo
 * para terminado
 * jogo: apontador para a estrutura que representa o jogo
 *
 */
void _Jogo_atualizar_estado(Jogo* jogo) {
	if(jogo->grelha->jogadas_restantes == 0)
		jogo->estado = JOGO_TERMINADO;
}

/*
 * Função que efetua uma jogada com um jogador artificial
 * jogo: apontador para a estrutura que representa o jogo
 *
 * Devolve o resultado da jogada
 */
EResultadoJogada Jogo_jogar_IA(Jogo* jogo) {
	// Por omissão
	EResultadoJogada resultado = JOGADA_INVALIDA;

	// Auxiliares para facilitar a escrita
	Grelha* grelha = jogo->grelha;
	int
		linha, linhas = grelha->linhas,
		coluna, colunas = grelha->colunas;

	// array de apontadores para estruturas de dados do tipo VetorCasas
	// o indice (0-2) representa o numero de lados já preenchidos nessas casas
	ListaCasas* casas_lados[3];
	int i;
	for(i = 0; i < 3; i++)
		casas_lados[i] = ListaCasas_new(linhas * colunas);

	// Apontador para a casa a jogar
	Casa* casa_jogar = NULL;
	for(linha = 0; linha < linhas; linha++) {
		for(coluna = 0; coluna < colunas; coluna++) {
			Casa* casa = grelha->casas[linha][coluna];
			// Caso tenha 3 lados é a casa a jogar
			if(casa->lados == 3) {
				casa_jogar = casa;
				// Sai do ciclo
				break;
			}
			// Caso contrário é adicionada à estrutura correspondente
			else if(casa->lados < 4)
				ListaCasas_add(casas_lados[casa->lados], casa);
		}
		// Caso tenha 3 lados aqui deixa de ser NULL e sai do ciclo
		if(casa_jogar != NULL)
			break;
	}

	for(i = 0; i < 3; i++) {
		// Se ainda não está definida a casa a jogar, atribuir aleatóriamente
		if(casa_jogar == NULL && ListaCasas_length(casas_lados[i]) > 0)
			casa_jogar = ListaCasas_get_random(casas_lados[i]);
		// Libertar o VetorCasas
		ListaCasas_free(casas_lados[i]);
	}

	ELado lado_jogar;

	if(casa_jogar->lados == 3) {
	// Se a casa a jogar tiver 3 lados preenchidos, jogar no lado disponivel
			lado_jogar =
				!casa_jogar->cima ? CIMA :
				!casa_jogar->esquerda ? ESQUERDA :
				!casa_jogar->baixo ? BAIXO :
				DIREITA;
	} else {
	// Se a casa a jogar tiver menos de 3 lados preenchidos, gerar numero
	// aleatorio:
	// - Se estiver apenas 1 lado preenchido, numero entre 0-1
	// - Caso contrário, 4 - nº de lados
		int aleatorio = rand() %
						(casa_jogar->lados == 1 ? 2 : 4 - casa_jogar->lados);

		if(casa_jogar->lados == 0 || casa_jogar->lados == 2) {
			switch(aleatorio) {
				// Para 0 lados cada valor corresponde a um lado, para 2 lados
				// na direção horizontal ou vertical é atribuido o lado oposto
				// caso o lado correspondente já esteja preenchido
				case 0:
					lado_jogar = CIMA;
					if(casa_jogar->cima)
						lado_jogar = BAIXO;
					break;
				case 1:
					lado_jogar = ESQUERDA;
					if(casa_jogar->esquerda)
						lado_jogar = DIREITA;
					break;
				case 2:
					lado_jogar = BAIXO;
					break;
				case 3:
					lado_jogar = DIREITA;
					break;
			}
		} else if(casa_jogar->lados == 1) {
			// Para casa com 1 lado preenchido, escolher a direção oposta e
			// selecionar o lado conforme o valor aleatório
			if(casa_jogar->cima || casa_jogar->baixo) {
				if(casa_jogar->esquerda || casa_jogar->direita)
					lado_jogar = aleatorio == 0 ? CIMA : BAIXO;
				else if(casa_jogar->cima || casa_jogar->baixo)
					lado_jogar = aleatorio == 0 ? ESQUERDA : DIREITA;
			}
		}
	}

	resultado = Grelha_jogar_int(grelha, casa_jogar->linha, casa_jogar->coluna,
											lado_jogar, jogo->jogador_atual);
	// Passar ao próximo jogador se a jogada for válida (captura volta a jogar)
	if(resultado == JOGADA_VALIDA)
		_Jogo_trocar_jogador(jogo);
	// Verificar se o jogo terminou
	_Jogo_atualizar_estado(jogo);

	return resultado;
}

/*
 * Função que efetua uma jogada numa determinada coordenada
 * jogo: apontador para a estrutura que representa o jogo
 * coordenada: apontador para a coordenada onde se vai efetuar a jogada
 *
 * Devolve o resultado da jogada
 */
EResultadoJogada Jogo_jogar(Jogo* jogo, Coordenada* coordenada) {
	EResultadoJogada resultado;
	resultado = Grelha_jogar_coordenada(jogo->grelha, coordenada,
														jogo->jogador_atual);
	// Passar ao próximo jogador se a jogada for válida (captura volta a jogar)
	if(resultado == JOGADA_VALIDA)
		_Jogo_trocar_jogador(jogo);
	// Verificar se o jogo terminou
	_Jogo_atualizar_estado(jogo);

	return resultado;
}

/*
 * Função que mostra o jogo em output
 * jogo: apontador para a estrutura que representa o jogo
 *
 */
void Jogo_print(Jogo* jogo) {
	// Mostra a grelha em output
	Grelha_print(jogo->grelha);
	int i;
	for(i = 0; i < jogo->numero_jogadores; i++) {
		// Mostra o simbolo e nº de casas de cada jogador
		Jogador* jogador = jogo->jogadores[i];
		printf("Casas %c: %d", jogador->simbolo, jogador->casas);
		if(i < jogo->numero_jogadores - 1)
			printf(", ");
		else
			printf("\n");
	}
	printf("\n");
}

#endif
