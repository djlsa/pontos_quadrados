/*
 * Coordenada.h
 * Representa uma coordenada introduzida por um jogador humano
 */

#ifndef COORDENADA_H_
#define COORDENADA_H_

#include <stdbool.h>
#include <ctype.h>

#include "Casa.h"

/*
 * Enumera��o das dire��es poss�veis representadas pela coordenada
 */
typedef enum {
	HORIZONTAL,
	VERTICAL
} EDirecao;

/*
 * Estrutura tipo Coordenada
 * linha, coluna: linha e coluna da grelha
 * direcao: dire��o da jogada
 */
typedef struct {
	int linha, coluna;
	EDirecao direcao;
} Coordenada;

// Vari�vel global que representa uma coordenada inv�lida
// Utiliza-se vari�vel em vez de constante para se poder retornar e fazer
// compara��es com refer�ncias � coordenada (&COORDENADA_INVALIDA)
Coordenada COORDENADA_INVALIDA = { -1, -1 };

/*
 * "Construtor" da estrutura tipo Coordenada
 * a, b: caracteres de input
 *
 * Devolve um apontador para a estrutura criada
 */
Coordenada* Coordenada_new(char a, char b) {
	// Colocam-se os caracteres em maiusculas
	a = toupper(a);
	b = toupper(b);

	// Se os caracteres estiverem a uma distancia inferior � distancia entre
	// os num�ricos e as letras maiusculas (ambos numericos ou letras), ou se
	// estiverem para al�m das letras, a coordenada � inv�lida
	if(abs(a - b) < '0' - 'A' - 1 || (a >= 'A' && b >= 'A') )
		return &COORDENADA_INVALIDA;

	// Verifica se o primeiro � maior que o segundo
	bool a_maior_b = a > b;
	// Aloca espa�o em mem�ria para armazenar a estrutura
	Coordenada* coordenada = malloc(sizeof(Coordenada));
	// Se o primeiro for maior, � esse que representa as linhas
	coordenada->linha = a_maior_b ? a - 'A' : b - 'A';
	// Se o segundo for maior, � o primeiro que representa as colunas
	coordenada->coluna = !a_maior_b ? a - '0' : b - '0';
	// Se o primeiro for maior, a dire��o � horizontal
	coordenada->direcao = a_maior_b ? HORIZONTAL : VERTICAL;

	// Caso um deles seja agora menor que zero, significa que estava abaixo dos
	// caracteres numericos
	if(coordenada->linha < 0 || coordenada->coluna < 0)
		return &COORDENADA_INVALIDA;

	return coordenada;
}

#endif
