/* 
 * File:   main.c
 * Author: Diogo
 *
 * Created on 19 de Novembro de 2010, 15:36
 */

#include <stdio.h>
#include <stdlib.h>


/**
 * Exemplo do jogo:
 *    x | o | x
 *   ---|---|---
 *    o | x |
 *   ---|---|---
 *    x | o |
 * 
 * Definições:
 *   - Jogada
 *      * 0 - espaço vazio
 *      * 1 - X
 *      * 2 - O
 *
 *   - Jogadores
 *      * 0 - CPU
 *      * 1 - Humano 1
 *      * 2 - Humano 2 (para multiplayer)
 */

/**
 * Guarda informações sobre a jogada
 */
struct jogada {
    int m[3][3]; // Matriz da jogada
    int ultJogador; // ultimo Jogador
    struct jogada *proxima; // Próxima jogada
};
typedef struct jogada tipoJogada;

/**
 * Armazena o histórico de jogadas (Pilha)
 */
struct historico {
    tipoJogada *primeira; // 1ª jogada
    int qtd; // Qtd de jogadas
};
typedef struct historico tipoHistorico;



int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}

