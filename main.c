/* 
 * File:   main.c
 *
 * Trabalho feito por:
 *  - Diogo Doreto
 *  - Gustavo Marçon
 *  - Vitor Pupio
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graficos.h"
#include "funcoes_aux.h"
#include "arvore_de_jogo.h"

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
 * Obs.:
 *  - Em singleplayer:
 *    > CPU      = VALOR_X
 *    > JOGADOR1 = VALOR_O
 * 
 *  - Em multiplayer:
 *    > JOGADOR1 = VALOR_X
 *    > JOGADOR2 = VALOR_O
 *
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

/** Funções principais do jogo */

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
        printf("\nEscolha o modo de jogo:\n\n"
                "  [0] Contra o PC;\n"
                "  [1] Contra outra pessoa;\n\n-> ");
        scanf(" %d", &modo);
        initJogo(&jogo, modo);

        do {
            leJogada(&jogo);
        } while (!verificaFim(&jogo));

        printf("\n** Fim de Jogo! **\n");
        imprimeJogo(jogo.historico.ultima->jogo);
        printf("\n** Fim de Jogo! **\n\n");
        switch (jogo.vencedor) {
            case CPU:
                printf("Que pena, o computador ganhou... tente de novo!");
                break;
            case JOGADOR1:
            case JOGADOR2:
                printf("Parabéns, jogador %d, você venceu!", jogo.vencedor);
                break;
            default:
                printf("Deu velha! :)");
        }

        printf("\nJogar de novo? [sim|nao]\n");
    } while (confirma());


    return (EXIT_SUCCESS);
}

/** Início das implementações da funções principais e das funções auxiliares */

void initJogo(tipoJogo *jogo, int modo) {
    jogo->modoJogo = modo;

    // Inicia primeira jogada (em branco)
    tipoJogada *jogada = (tipoJogada*) malloc(sizeof(tipoJogada));
    
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

/**
 * Analiza as posições livres do jogo e retorna uma aleatória (não utilizado no
 * momento, mas pode ser usado para um nível de dificuldade fácil)
 * @param jogo Matriz contendo a ultima jogada
 * @return Posição livre aleatória
 */
int* _calculaPosicaoLivre(int jogo[3][3]) {
    int *posicoesLivres = (int*) malloc(sizeof(int*)*9);
    int i = -1, j, p = 0;

    // Procura e guarda as posições livres no vetor {posicoesLivres[]}
    while (++i < 3) {
        j = -1;
        while (++j < 3) {
            if (0 == jogo[i][j]) {
                posicoesLivres[p++] = &jogo[i][j];
            }
        }
    }

    // Sorteia uma posição e a retorna
    return posicoesLivres[aleatorio(0, p-1)];
}

/**
 * Cria uma jogada nova no histórico de jogadas
 * @param hist Histórico de jogadas
 */
void _novaJogada(tipoHistorico *hist) {
    tipoJogada *novaJogada = (tipoJogada*) malloc(sizeof(tipoJogada));

    // Clona jogada
    int i = -1, j;
    while (++i < 3) {
        j = -1;
        while (++j < 3) {
            novaJogada->jogo[i][j] = hist->ultima->jogo[i][j];
        }
    }

    novaJogada->anterior = hist->ultima;
    hist->ultima = novaJogada;
    hist->qtd++;
}

/**
 * Faz a leitura da jogada para o modo MULTIPLAYER.
 * Retorna os valores para os ponteiros {lin} e
 * {col} passados.
 * @param jogada Ultima jogada do histórico
 * @param lin Ponteiro que receberá a linha da jogada
 * @param col Ponteiro que receberá a coluna da jogada
 */
void _leAcaoJogadorMultip(tipoJogada *jogada, int *lin, int *col) {
    (*lin) = -1;
    do {
        if ((*lin) != -1) {
            printf("\n** Esta posição não está disponível! **\n\n");
        }
        imprimeJogo(jogada->jogo);
        do {
            printf("\n\nDigite a linha e a coluna que deseja realizar a jogada.\n-> ");
            scanf(" %d %d", lin, col);
        } while (*lin < 1 || *lin > 3 || *col < 1 || *col > 3);
        (*lin)--;
        (*col)--;
        printf("\n");
    } while (VALOR_VAZIO != jogada->jogo[*lin][*col]);
}

/**
 * Volta uma jogada do histórico (remove da pilha)
 * @param hist Histórico de jogadas
 */
void _voltaJogada(tipoHistorico *hist) {
    tipoJogada *temp = hist->ultima;
    hist->ultima = temp->anterior;
    free(temp);
}

/**
 * Imprime todas as possibilidades da próxima jogada a partir do jogo passado
 * como parâmetro.
 * @param intbrd Matriz contendo uma jogada
 */
void _imprimePossibilidades(int intbrd[3][3]) {
    printf("\n\nDeseja ver a lista de jogadas possíveis? [sim|nao]\n");
    if (!confirma()) return;
    // Jogador quis ver as possibilidades, então continua o código:
    char brd[3][3];
    int tmpbrd[3][3];
    _converteMatrizIntParaChar(intbrd, brd);
    noPtr arvPossib = buildtree(brd, 1, 'o');
    noPtr no = arvPossib->son;
    while (no != NULL) {
        _converteMatrizCharParaInt(no->board, tmpbrd);
        printf("\n");
        imprimeJogo(tmpbrd);
        no = no->next;
        if (no != NULL) {
            printf("\nDeseja ver a próxima jogada possível? [sim|nao]\n");
            if (!confirma()) break;
        } else
            printf("\n");
    }
    printf("\n** Jogo atual:\n\n");
    imprimeJogo(intbrd);
    printf("\n");
}

void _leAcaoJogadorSinglep(tipoHistorico *hist, int *lin, int *col) {
    char act;
    (*lin) = (*col) = -1;

    do {
        if ((*lin) != -1) { // Não é a 1ª tentativa de jogar
            printf("\n** Esta posição não está disponível! **\n\n");
        }
        imprimeJogo(hist->ultima->jogo);
        _imprimePossibilidades(hist->ultima->jogo);
        do {
            printf("\n\nDigite a linha e a coluna que deseja realizar a jogada");
            if (hist->qtd > 3) {
                printf(",\nou a letra 'z' para voltar uma jogada");
            }
            printf(".\n-> ");
            
            scanf(" %c", &act);
            if (act == 'z') {
                if (hist->qtd > 3) {
                    _voltaJogada(hist); // Volta a jogada 'nova'
                    _voltaJogada(hist); // Volta a jogada do CPU
                    _voltaJogada(hist); // Volta a jogada anterior do JOGADOR1
                    _novaJogada(hist);  // Cria outra jogada 'nova'
                    printf("\n");
                    imprimeJogo(hist->ultima->jogo);
                }
                (*lin) = 0;
            } else {
                *lin = atoi(&act);
                scanf(" %d", col);
            }
        } while (*lin < 1 || *lin > 3 || *col < 1 || *col > 3);
        (*lin)--;
        (*col)--;
        printf("\n");
    } while (VALOR_VAZIO != hist->ultima->jogo[*lin][*col]);
}

void _converteMatrizIntParaChar(int origem[3][3], char destino[3][3]) {
    int lin, col;
    for (lin = 0; lin < 3; ++lin)
        for (col = 0; col < 3; ++col)
            destino[lin][col] = origem[lin][col] == VALOR_X ? 'x'
                : (origem[lin][col] == VALOR_O ? 'o' : '-');
}

void _converteMatrizCharParaInt(char origem[3][3], int destino[3][3]) {
    int lin, col;
    for (lin = 0; lin < 3; ++lin)
        for (col = 0; col < 3; ++col)
            destino[lin][col] = origem[lin][col] == 'x' ? VALOR_X
                : (origem[lin][col] == 'o' ? VALOR_O : VALOR_VAZIO);
}

void leJogada(tipoJogo *jogo) {
    int lin, col;
    // Gera a nova jogada
    _novaJogada(&jogo->historico);
    if (jogo->modoJogo == SINGLEPLAYER) {
        if (JOGADOR1 == jogo->historico.ultima->anterior->ultJogador) {
            // Vez do computador
            printf("O computador (x) está pensando...\n\n");

            char brdconv[3][3], newbrd[3][3];
            // converte o tabuleiro de int para char
            _converteMatrizIntParaChar(jogo->historico.ultima->jogo, brdconv);

            // Calcula a próxima jogada
            nextmove(brdconv, 1, 'x', newbrd);

            // desconverte {newbrd} de char para int
            _converteMatrizCharParaInt(newbrd, jogo->historico.ultima->jogo);

            jogo->historico.ultima->ultJogador = CPU;
        } else {
            // Vez do jogador 1
            printf("* Vez do jogador 1 (o):\n\n");
            
            _leAcaoJogadorSinglep(&(jogo->historico), &lin, &col);

            jogo->historico.ultima->jogo[lin][col] = VALOR_O;
            jogo->historico.ultima->ultJogador = JOGADOR1;
        }
    } else { // MULTIPLAYER
        if (JOGADOR2 == jogo->historico.ultima->anterior->ultJogador) {
            // Vez do JOGADOR1
            printf("* Vez do jogador 1 (x):\n\n");
            _leAcaoJogadorMultip(jogo->historico.ultima, &lin, &col);

            jogo->historico.ultima->jogo[lin][col] = VALOR_X;
            jogo->historico.ultima->ultJogador = JOGADOR1;
        } else {
            // Vez do JOGADOR2
            printf("* Vez do jogador 2 (o):\n\n");
            _leAcaoJogadorMultip(jogo->historico.ultima, &lin, &col);

            jogo->historico.ultima->jogo[lin][col] = VALOR_O;
            jogo->historico.ultima->ultJogador = JOGADOR2;
        }
    }
}

int verificaFim(tipoJogo *jogo) {
    int i = -1;
    while (++i < 3) {
        // Checa linha {i}
        if (jogo->historico.ultima->jogo[i][0] ==
                jogo->historico.ultima->jogo[i][1] &&
                jogo->historico.ultima->jogo[i][1] ==
                jogo->historico.ultima->jogo[i][2] &&
                jogo->historico.ultima->jogo[i][0] != VALOR_VAZIO) {
            if (jogo->modoJogo == SINGLEPLAYER) {
                jogo->vencedor = jogo->historico.ultima->jogo[i][0] == VALOR_X ?
                    CPU : JOGADOR1;
            } else {
                jogo->vencedor = jogo->historico.ultima->jogo[i][0] == VALOR_X ?
                    JOGADOR1 : JOGADOR2;
            }
            return 1;
        }
        // Checa coluna {i}
        if (jogo->historico.ultima->jogo[0][i] ==
                jogo->historico.ultima->jogo[1][i] &&
                jogo->historico.ultima->jogo[1][i] ==
                jogo->historico.ultima->jogo[2][i] &&
                jogo->historico.ultima->jogo[0][i] != VALOR_VAZIO) {
            if (jogo->modoJogo == SINGLEPLAYER) {
                jogo->vencedor = jogo->historico.ultima->jogo[0][i] == VALOR_X ?
                    CPU : JOGADOR1;
            } else {
                jogo->vencedor = jogo->historico.ultima->jogo[0][i] == VALOR_X ?
                    JOGADOR1 : JOGADOR2;
            }
            return 1;
        }
    }
    // Checa uma diagonal
    if (jogo->historico.ultima->jogo[0][0] ==
            jogo->historico.ultima->jogo[1][1] &&
            jogo->historico.ultima->jogo[1][1] ==
            jogo->historico.ultima->jogo[2][2] &&
            jogo->historico.ultima->jogo[0][0] != VALOR_VAZIO) {
        if (jogo->modoJogo == SINGLEPLAYER) {
            jogo->vencedor = jogo->historico.ultima->jogo[0][0] == VALOR_X ?
                    CPU : JOGADOR1;
        } else {
            jogo->vencedor = jogo->historico.ultima->jogo[0][0] == VALOR_X ?
                    JOGADOR1 : JOGADOR2;
        }
        return 1;
    }
    // Checa outra diagonal
    if (jogo->historico.ultima->jogo[0][2] ==
            jogo->historico.ultima->jogo[1][1] &&
            jogo->historico.ultima->jogo[1][1] ==
            jogo->historico.ultima->jogo[2][0] &&
            jogo->historico.ultima->jogo[0][2] != VALOR_VAZIO) {
        if (jogo->modoJogo == SINGLEPLAYER) {
            jogo->vencedor = jogo->historico.ultima->jogo[0][2] == VALOR_X ?
                    CPU : JOGADOR1;
        } else {
            jogo->vencedor = jogo->historico.ultima->jogo[0][2] == VALOR_X ?
                    JOGADOR1 : JOGADOR2;
        }
        return 1;
    }
    // Checa se deu velha
    if (jogo->historico.qtd > 9) {
        jogo->vencedor = EMPATE;
        return 1;
    }
    // O jogo ainda não acabou
    return 0;
}
