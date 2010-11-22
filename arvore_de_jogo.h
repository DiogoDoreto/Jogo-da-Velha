/* 
 * File:   arvore.h
 */

#ifndef ARVORE_H
#define	ARVORE_H

#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Estrutura do nó da árvore de jogo
     */
    typedef struct nodetype {
        char board[3][3];
        int turn;
        struct nodetype *son;
        struct nodetype *next;
    } *noPtr;

    /**
     * Função de criação de nó (aloca um espaço de memória para ele)
     * @return Nó devidamente alocado
     */
    noPtr getnode() {
        return (noPtr) malloc(sizeof(struct nodetype));
    }

    /**
     * Gera todas as posições de tabuleiro que podem ser obtidas a partir de
     * {brd}
     * @param brd
     * @return Lista dos nós gerados
     */
    noPtr generate(char brd[3][3], char player) {
        noPtr firstNode = NULL, p = NULL;
        int i, j, k, l;
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if ('-' == brd[i][j]) {
                    if (p == NULL) {
                        // {p} vazio, então é o 1º elemento da lista
                        firstNode = p = getnode();
                    } else {
                        // {p} nao vazio, então cria novo elemento na lista
                        p->next = getnode();
                        p = p->next;
                    }
                    // Clona o tabuleiro atual...
                    for (k = 0; k < 3; k++)
                        for (l = 0; l < 3; l++)
                            p->board[k][l] = brd[k][l];
                    // ... e joga o player na posição vazia
                    p->board[i][j] = player;
                }
            }
        }
        //Termina o encadeamento
        if (p != NULL)
            p->next = NULL;
        return firstNode;
    }

    /**
     * Gera todas as posições do tabuleiro que podem ser obtidas a partir da
     * posição do tabuleiro apontada por {p} e estabelecendo-as como filhos de
     * {p} na árvore de jogo. Em seguida {expand} chama a si mesma,
     * recursivamente, usando esses filhos como param. até que a profundidade
     * desejada seja alcançada.
     * @param p
     * @param plevel
     * @param depth
     * @param player
     */
    void expand(noPtr p, int plevel, int depth, char player) {
        noPtr q;
        if (plevel < depth) {
            // p não está no nível máximo
            q = generate(p->board, player);
            p->son = q;
            // Percorre a lista de nós
            while (q != NULL) {
                q->turn = p->turn == 1 ? -1 : 1;
                q->son = NULL;
                expand(q, plevel+1, depth, player == 'x' ? 'o' : 'x');
                q = q->next;
            }
        }
    }

    /**
     * Constrói a árvore de jogo
     * @param brd
     * @param looklevel
     * @param player
     * @return Ponteiro para a raiz de uma árvore de jogo.
     */
    noPtr buildtree(char brd[3][3], int looklevel, char player) {
        noPtr ptree;
        int i, j;
        //Cria a raiz da arvore e a inicializa
        ptree = getnode();
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j)
                ptree->board[i][j] = brd[i][j];
        // A raiz é um nó de 'mais' por definição
        ptree->turn = 1;
        ptree->son = NULL;
        ptree->next = NULL;
        // Cria o restante da árvore de jogo
        expand(ptree, 0, looklevel, player);
        return ptree;
    }

    /**
     * Avalia o quão boa é uma jogada para {player}.
     * O cálculo se dá por contar quantas colunas, linhas e diagonais estão
     * abertas para {player} e subtrair pela quantidade do adversário.
     * @param brd
     * @param pvalue
     * @param player
     * @return Valor da avaliação
     */
    int evaluate(char brd[3][3], int *pvalue, char player) {
        int val = 0, i;
        player = 'x';
        char adv = player == 'x' ? 'o' : 'x';

        for (i = 0; i < 3; ++i) {
            // Analiza linha {i}
            if (brd[i][0] != adv && brd[i][1] != adv && brd[i][2] != adv)
                val++;

            if (brd[i][0] != player && brd[i][1] != player && brd[i][2] != player) {
                val--;
                if ((brd[i][0] == '-' && brd[i][1] != '-' && brd[i][2] != '-') ||
                        (brd[i][0] != '-' && brd[i][1] == '-' && brd[i][2] != '-') ||
                        (brd[i][0] != '-' && brd[i][1] != '-' && brd[i][2] == '-'))
                   return *pvalue = -9; // Jogada perdedora!
            }

            if (brd[i][0] == player && brd[i][1] == player && brd[i][2] == player)
                return *pvalue = 9; // Jogada vencedora!

            // Analiza coluna {i}
            if (brd[0][i] != adv && brd[1][i] != adv && brd[2][i] != adv)
                val++;

            if (brd[0][i] != player && brd[1][i] != player && brd[2][i] != player) {
                val--;
                if ((brd[0][i] == '-' && brd[1][i] != '-' && brd[2][i] != '-') ||
                        (brd[0][i] != '-' && brd[1][i] == '-' && brd[2][i] != '-') ||
                        (brd[0][i] != '-' && brd[1][i] != '-' && brd[2][i] == '-'))
                   return *pvalue = -9; // Jogada perdedora!
            }

            if (brd[0][i] == player && brd[1][i] == player && brd[2][i] == player)
                return *pvalue = 9; // Jogada vencedora!
        }
        // Analiza uma diagonal
        if (brd[0][0] != adv && brd[1][1] != adv && brd[2][2] != adv)
            val++;

        if (brd[0][0] != player && brd[1][1] != player && brd[2][2] != player) {
            val--;
            if ((brd[0][0] == '-' && brd[1][1] != '-' && brd[2][2] != '-') ||
                    (brd[0][0] != '-' && brd[1][1] == '-' && brd[2][2] != '-') ||
                    (brd[0][0] != '-' && brd[1][1] != '-' && brd[2][2] == '-'))
                return *pvalue = -9; // Jogada perdedora!
        }

        if (brd[0][0] == player && brd[1][1] == player && brd[2][2] == player)
            return *pvalue = 9; // Jogada vencedora!

        // Analiza outra diagonal
        if (brd[0][2] != adv && brd[1][1] != adv && brd[0][2] != adv)
            val++;

        if (brd[0][2] != player && brd[1][1] != player && brd[0][2] != player) {
            val--;
            if ((brd[0][2] == '-' && brd[1][1] != '-' && brd[2][0] != '-') ||
                    (brd[0][2] != '-' && brd[1][1] == '-' && brd[2][0] != '-') ||
                    (brd[0][2] != '-' && brd[1][1] != '-' && brd[2][0] == '-'))
                return *pvalue = -9; // Jogada perdedora!
        }

        if (brd[0][2] == player && brd[1][1] == player && brd[0][2] == player)
            return *pvalue = 9; // Jogada vencedora!

        return *pvalue = val;
    }

    /**
     * Avalia os nós das árvores
     * @param pnd
     * @param player
     * @param pbest Ponteiro para o filho da raiz da árvore que maximiza seu valor
     * @param pvalue Valor do {pbest}
     */
    void bestbranch(noPtr pnd, char player, noPtr *pbest, int *pvalue) {
        noPtr p, pbest2;
        int val;

        if (pnd->son == NULL) {
            // {pnd} é folha
            *pvalue = evaluate(pnd->board, pvalue, player);
            *pbest = pnd;
        } else {
            // {pnd} não é folha, atravesse a lista de filhos
            p = pnd->son;
            bestbranch(p, player, pbest, pvalue);
            *pbest = p;
            if (pnd->turn == -1) {
                *pvalue = -*pvalue;
            }
            p = p->next;
            while (p != NULL) {
                bestbranch(p, player, &pbest2, &val);
                if (pnd->turn == -1) {
                    val = -val;
                }
                if (val > *pvalue) {
                    *pvalue = val;
                    *pbest = p;
                }
                p = p->next;
            }
            if (pnd->turn == -1)
                *pvalue = -*pvalue;
        }
    }

    /**
     * Calcula a melhor jogada seguinte
     * @param brd Vetor que representa a atual posição do tabuleiro
     * @param looklevel Nível de previsão usado ao construir a árvore
     * @param player 'x' ou 'o'
     * @param newbrd Param. de saída q representa a melhor posição do tabuleiro
     *               que pode ser alcançada por {player} a partir de {brd}
     */
    void nextmove(char brd[3][3], int looklevel, char player, char newbrd[3][3]) {
        noPtr ptree, best;
        int i, j, value;
        ptree = buildtree(brd, looklevel, player);
        bestbranch(ptree, player, &best, &value);
        for (i = 0; i < 3; ++i)
            for (j = 0; j < 3; ++j)
                newbrd[i][j] = best->board[i][j];
    }


#ifdef	__cplusplus
}
#endif

#endif	/* ARVORE_H */

