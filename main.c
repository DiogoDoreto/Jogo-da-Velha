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

#define EMPATE -1

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
    int vencedor;
} tipoJogo;

/**
 * Inicia o jogo
 * @param jogo Jogo a ser iniciado
 * @param modo Modo do jogo [ SINGLEPLAYER | MULTIPLAYER ]
 */
void initJogo(tipoJogo *jogo, int modo);

/**
 * Faz a leitura de uma nova jogada
 * @param jogo
 */
void leJogada(tipoJogo *jogo);

/**
 * Verifica se é o final da partida, ou seja, se houve empate ou vitória.
 * @param jogo
 * @return 1 se for o fim e 0 se o jogo deve continuar
 */
int verificaFim(tipoJogo *jogo);

int main(int argc, char** argv) {
    tipoJogo jogo;
    int modo;

    do {
        printf("Escolha o modo de jogo:\n\n"
                "  [0] Contra o PC;\n"
                "  [1] Contra outra pessoa;\n\n-> ");
        scanf(" %d", &modo);
        initJogo(&jogo, modo);

        do {
            leJogada(&jogo);
        } while (!verificaFim(&jogo));

        printf("\n** Fim de Jogo!\n");
        switch (jogo->vencedor) {
            case CPU:
                printf("Que pena, o computador ganhou... tente de novo!");
                break;
            case JOGADOR1:
            case JOGADOR2:
                printf("Parabéns, jogador %d, você venceu!", jogo->vencedor);
                break;
            default:
                printf("Deu velha! :)");
        }

        printf("\nJogar de novo? [sim|nao]\n");
    } while (confirma());


    return (EXIT_SUCCESS);
}

/** Início das implementações */

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
    if (modo == SINGLEPLAYER)
        jogada->ultJogador = aleatorio(0, 1);
    else
        jogada->ultJogador = aleatorio(1, 2);

    jogada->anterior = NULL;

    jogo->vencedor = NULL;
    jogo->historico.ultima = jogada;
    jogo->historico.qtd = 1;
}

int*** _calculaPosicaoLivre(int *jogo[3][3]) {
    int **posicoesLivres[9];
    int i = -1, j, p = 0;

    // Procura e guarda as posições livres no vetor {posicoesLivres[]}
    while (++i < 3) {
        j = -1;
        while (++j < 3) {
            if (0 == *jogo[i][j]) {
                posicoesLivres[p++] = jogo[i][j];
            }
        }
    }

    // Sorteia uma posição e a retorna
    return posicoesLivres[aleatorio(0, p-1)];
}

void leJogada(tipoJogo *jogo) {
    if (jogo->modoJogo == SINGLEPLAYER) {
        if (JOGADOR1 == jogo->historico.ultima->ultJogador) {
            // Vez do computador
            printf("O computador está pensando...\n\n");

            // Calcula uma posição livre aleatoria para jogada
            int ***posicao = _calculaPosicaoLivre(&jogo->historico.ultima->jogo);
        }
    }
}

int verificaFim(tipoJogo *jogo) {

}
