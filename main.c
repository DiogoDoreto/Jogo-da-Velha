/* 
 * File:   main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graficos.h"
#include "funcoes_aux.h"

/** Definições de constantes utilizadas no jogo */

#define VALOR_VAZIO 0
#define VALOR_X 1
#define VALOR_O 2

#define SINGLEPLAYER 0
#define MULTIPLAYER 1

#define CPU 0
#define JOGADOR1 1
#define JOGADOR2 2

/**
 * Exemplo do jogo:
 *    x | o | x
 *   ---|---|---
 *    o | x |
 *   ---|---|---
 *    x | o |
 */

/**
 * Guarda informações sobre a jogada
 */
typedef struct jogada {
    int jogo[3][3]; // Matriz da jogada
    int ultJogador; // ultimo Jogador
    struct jogada *anterior; // Jogada Anterior
} tipoJogada;

/**
 * Armazena o histórico de jogadas (Pilha com poteiro sempre na última posição)
 */
typedef struct historico {
    tipoJogada *ultima; // Última jogada
    int qtd; // Qtd de jogadas
} tipoHistorico;

/**
 * Armazena o jogo e suas propriedades
 */
typedef struct jogo {
    tipoHistorico historico;
    int modoJogo; // SINGLEPLAYER ou MULTIPLAYER
} tipoJogo;

/**
 * Inicia o jogo
 * @param jogo Jogo a ser iniciado
 * @param modo Modo do jogo [ SINGLEPLAYER | MULTIPLAYER ]
 */
void initJogo(tipoJogo *jogo, int modo);

int main(int argc, char** argv) {
    tipoJogo jogo;
    int modo;

    do {
        printf("Escolha o modo de jogo:\n\n"
                "  [0] Contra o PC;\n"
                "  [1] Contra outra pessoa;\n\n-> ");
        scanf(" %d", &modo);
        initJogo(&jogo, modo);


        printf("\nJogar de novo? [sim|nao]\n");
    } while (confirma());


    return (EXIT_SUCCESS);
}

void initJogo(tipoJogo *jogo, int modo) {
    jogo->modoJogo = modo;

    // Inicia primeira jogada (em branco)
    tipoJogada *jogada = malloc(sizeof(tipoJogada));
    
    // Inicializa matriz do jogo
    int i = -1, j;
    while (++i < 3)
        while (++j < 3)
            jogada->jogo[i][j] = 0;

    // Sorteia quem irá iniciar o jogo
    srand((unsigned int) time(NULL));
    int jogador = (int) (rand() % 2);
    jogada->ultJogador = jogador;

    jogada->anterior = NULL;

    jogo->historico.ultima = jogada;
    jogo->historico.qtd = 1;
}

