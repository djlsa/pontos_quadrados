/*
 ============================================================================
 Name		 : Programa��o, e-F�lio A
 Author		 : David Salsinha
 Description : Jogo dos pontos e quadrados, alinea D
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "Jogo.h"

int main(int argc, char** argv) {
	printf("Jogo dos pontos e quadrados\n");

	// Por omiss�o 2 jogadores humanos e 0 artificiais
	int
		total_jogadores = 2,
		jogadores_ai = 0;

	// Caso exista pelo menos 1 argumento al�m do comando
	if(argc >= 2) {
		// Converte para inteiro
		int jogadores = atoi(argv[1]);
		if(jogadores > 1 && jogadores <= JOGO_MAXIMO_JOGADORES)
			total_jogadores = jogadores;
		// Caso existam pelo menos 2 argumentos
		if(argc >= 3) {
			int jog_ai = atoi(argv[2]);
			if(jog_ai > 0 && jogadores_ai <= JOGO_MAXIMO_JOGADORES)
				jogadores_ai = jog_ai;
		}
	}

	// Como a dimens�o � apenas 1 algarismo, � mais simples utilizar
	// um char
	char dimensao;
	do {
		printf("Indique dimensao [2-9]: ");
		// Ler o char
		dimensao = fgetc(stdin);
		// Caso n�o tenha sido um ENTER
		if(dimensao != '\n')
			//ignorar todos os caracteres seguintes at� ao pr�ximo ENTER
			while(fgetc(stdin) != '\n');
	} while(dimensao < '2' || dimensao > '9');

	printf("\n");

	// Converter para o valor inteiro correto
	dimensao -= '0';

	// Inicializa o jogo e mostra o estado inicial em output
	Jogo* jogo = Jogo_new(dimensao, dimensao, total_jogadores, jogadores_ai);
	Jogo_print(jogo);

	// Apontador para coordenada, por omiss�o refer�ncia � vari�vel global que
	// representa uma coordenada inv�lida
	Coordenada* coordenada = &COORDENADA_INVALIDA;

	EResultadoJogada resultado;
	char a, b;

	// Repetir at� o jogo terminar
	while(jogo->estado != JOGO_TERMINADO) {
		// Caso o jogador atual seja artificial, efetuar a jogada
		while(jogo->jogador_atual->tipo == JOGADOR_ARTIFICIAL &&
											jogo->estado != JOGO_TERMINADO) {
			Jogo_jogar_IA(jogo);
			Jogo_print(jogo);
		}
		// Por omiss�o a jogada do jogador humano � inv�lida
		resultado = JOGADA_INVALIDA;
		// Repetir enquanto a jogada for inv�lida ou ocorrer uma captura
		while(resultado != JOGADA_VALIDA && jogo->estado != JOGO_TERMINADO) {
			do {
				printf("Jogada de %c (B3 traco horizontal da linha B e coluna",
												jogo->jogador_atual->simbolo);
				printf(" 3, 3B traco vertical): ");
				// Ler um caractere
				a = fgetc(stdin);
				// Caso n�o tenha sido um ENTER
				if(a != '\n') {
					// Ler o 2� caractere
					b = fgetc(stdin);
					// Inicializar a coordenada
					coordenada = Coordenada_new(a, b);
					// Caso o 2� tamb�m n�o tenha sido um ENTER, ignorar todos
					// os seguintes at� ao pr�ximo ENTER
					if(b != '\n')
						while(fgetc(stdin) != '\n');
				}
			// Repetir enquanto a coordenada for inv�lida
			} while(coordenada == &COORDENADA_INVALIDA);

			resultado = Jogo_jogar(jogo, coordenada);
			// Libertar a mem�ria usada pela coordenada
			free(coordenada);
			// Se a jogada for v�lida ou ocorrer captura, mostrar no ecr�
			if(resultado != JOGADA_INVALIDA)
				Jogo_print(jogo);
		}
		printf("\n");
	}

	// Libertar a mem�ria usada pelo jogo
	Jogo_free(jogo);

	return EXIT_SUCCESS;
}
