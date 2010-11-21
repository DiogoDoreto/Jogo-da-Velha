/* 
 * File:   funcoes_aux.h
 */

#ifndef FUNCOES_AUX_H
#define	FUNCOES_AUX_H

#include <ctype.h>

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * Aceita [sim|nao] como resposta e retorna de acordo
     * @return 1 para "sim" e 0 para "nao"
     */
    int confirma() {
        char *resp = (char *) malloc(sizeof (char *) * 4);
        printf("-> ");
        scanf(" %3s", resp);

        // Transforma a resposta em caixa baixa
        int i = -1;
        while (resp[++i])
            resp[i] = tolower(resp[i]);

        return strcmp(resp, "sim") == 0 ? 1 : 0;
    }

    /**
     * Retorna um número inteiro aleatório
     * @param min Menor número para retorno
     * @param max Maior número para retorno
     * @return Número aleatório entre {min} e {max}
     */
    int aleatorio(int min, int max) {
        srand((unsigned int) time(NULL));
        int num = (int) (rand() % (max + 1 - min)) + min;
        return num;
    }


#ifdef	__cplusplus
}
#endif

#endif	/* FUNCOES_AUX_H */

