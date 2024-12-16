#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assembler.h"

int main(int argc, char *argv[]) {
    printf("Numero di argomenti: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argomento %d: %s\n", i, argv[i]);
    }
    if (argc >= 2) {
        // Apertura del file .asm
        FILE *asmfile = fopen(argv[1], "r");

        if (asmfile == NULL) {
            printf("Errore in apertura del file .asm: %s\n", argv[1]);
            exit(1);
        }

        FILE *hackfile;
        char filename[64];
        strcpy(filename, argv[1]);

        // Ci aspettiamo un .asm quindi rimuoviamo 3 caratteri.
        filename[strlen(filename) - 3] = '\0';
        strcat(filename, "hack"); // Concatenazione

        hackfile = fopen(filename, "w");
        if (hackfile == NULL) {
            printf("Errore di lettura file .hack\n");
        }

        // Scrive direttamente sul file
        assemble(asmfile, hackfile);
        fclose(asmfile);
        fclose(hackfile);
        printf("Processo terminato\n");
    } else {
        printf("Uso: %s [filename]\n", argv[0]);
    }
    return 0;
}
