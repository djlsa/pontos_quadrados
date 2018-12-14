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
 * Enumeração das direções possíveis representadas pela coordenada
 */
typedef enum {
	HORIZONTAL,
	VERTICAL
} EDirecao;

/*
 * Estrutura tipo Coordenada
 * linha, coluna: linha e coluna da grelha
 * direcao: direção da jogada
 */
typedef struct {
	int linha, coluna;
	EDirecao direcao;
} Coordenada;

// Variável global que representa uma coordenada inválida
// Utiliza-se variável em vez de constante para se poder retornar e fazer
// comparações com referências à coordenada (&COORDENADA_INVALIDA)
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

	// Se os caracteres estiverem a uma distancia inferior à distancia entre
	// os numéricos e as letras maiusculas (ambos numericos ou letras), ou se
	// estiverem para além das letras, a coordenada é inválida
	if(abs(a - b) < '0' - 'A' - 1 || (a >= 'A' && b >= 'A') )
		return &COORDENADA_INVALIDA;

	// Verifica se o primeiro é maior que o segundo
	bool a_maior_b = a > b;
	// Aloca espaço em memória para armazenar a estrutura
	Coordenada* coordenada = malloc(sizeof(Coordenada));
	// Se o primeiro for maior, é esse que representa as linhas
	coordenada->linha = a_maior_b ? a - 'A' : b - 'A';
	// Se o segundo for maior, é o primeiro que representa as colunas
	coordenada->coluna = !a_maior_b ? a - '0' : b - '0';
	// Se o primeiro for maior, a direção é horizontal
	coordenada->direcao = a_maior_b ? HORIZONTAL : VERTICAL;

	// Caso um deles seja agora menor que zero, significa que estava abaixo dos
	// caracteres numericos
	if(coordenada->linha < 0 || coordenada->coluna < 0)
		return &COORDENADA_INVALIDA;

	return coordenada;
}

#endif
