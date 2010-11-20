/* 
 * File:   funcoes_aux.h
 */

#ifndef FUNCOES_AUX_H
#define	FUNCOES_AUX_H

#include <ctype.h>

#ifdef	__cplusplus
extern "C" {
#endif

    int confirma() {
        char *resp = (char) malloc(sizeof(char) * 4);
        printf("-> ");
        scanf(" %3s", resp);

        // Transforma a resposta em caixa baixa
        int i = -1;
        while (resp[++i])
            resp[i] = tolower(resp[i]);

        return strcmp(resp, "sim") == 0 ? 1 : 0;
    }


#ifdef	__cplusplus
}
#endif

#endif	/* FUNCOES_AUX_H */

