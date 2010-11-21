/* 
 * File:   graficos.h
 */

#ifndef GRAFICOS_H
#define	GRAFICOS_H

#define VALOR_VAZIO 0
#define VALOR_X 1
#define VALOR_O 2

#ifdef	__cplusplus
extern "C" {
#endif

    char* _valorPosicao(int v) {
        switch (v) {
            case VALOR_VAZIO:
                return " ";
                break;
            case VALOR_X:
                return "x";
                break;
            case VALOR_O:
                return "o";
                break;
        }
    }

    /**
     * Imprime o jogo na tela
     * @param m Matriz contendo a jogada atual
     */
    void imprimeJogo(int m[3][3]) {
        int i = -1, j;
        while (++i < 3) {
            j = -1;
            while (++j < 3) {
                printf(" %s ", _valorPosicao(m[i][j]));
                if (j != 2)
                    printf("|");
            }
            if (i != 2)
                printf("\n---|---|---\n");
        }
    }


#ifdef	__cplusplus
}
#endif

#endif	/* GRAFICOS_H */

