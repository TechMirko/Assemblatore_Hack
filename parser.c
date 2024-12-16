#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "symbolTable.h"

tabella compTable = {
    37, {
        {"0",   42},
        {"1",   63},
        {"-1",  58},
        {"D",   12},
        {"A",   48},
        {"M",   112},
        {"!D",  13},
        {"!A",  49},
        {"!M",  113},
        {"-D",  15},
        {"-A",  51},
        {"-M",  115},
        {"D+1", 31},
        {"A+1", 55},
        {"M+1", 119},
        {"D-1", 14},
        {"A-1", 50},
        {"M-1", 114},
        {"D+A", 2},
        {"D+M", 66},
        {"D-A", 19},
        {"D-M", 83},
        {"A-D", 7},
        {"M-D", 71},
        {"D&A", 0},
        {"D&M", 64},
        {"D|A", 21},
        {"D|M", 85}
    }
};

tabella jumpTable = {
    8, {
        {"null", 0},
        {"JGT", 1},
        {"JEQ", 2},
        {"JGE", 3},
        {"JLT", 4},
        {"JNE", 5},
        {"JLE", 6},
        {"JMP", 7}
    }
};

tabella destTable = {
    8, {
        {"null", 0},
        {"M",   1},
        {"D",   2},
        {"MD",  3},
        {"A",   4},
        {"AM",  5},
        {"AD",  6},
        {"AMD", 7}
    }
};

instruction* parseInstruction(char* riga) {
    instruction* instr = NULL;

    if (riga[0] == '@') { //è una A-instruction
        if (riga[1] <= '9' && riga[1] >= '0') {
            instr = (instruction*)malloc(sizeof(instruction));
            instr->type = A;

            unsigned short indirizzo;
            if (sscanf(riga, "@%hu", &indirizzo) == 0) {
                printf("Errore nel file asm: %s\n", riga);
                free(instr);
                exit(1);
            }

            if (indirizzo >= (1 << 15)) {
                printf("Errore, indirizzo fuori range %hu\n", indirizzo);
            }

            instr->valore = indirizzo;
        }
    } else {
        unsigned char comp, dest, jump;
        parseC(riga, &comp, &dest, &jump);

        if (dest == MISSING) dest = 0;
        if (jump == MISSING) jump = 0;

        if (comp == ERROR || dest == ERROR || jump == ERROR) {
            printf("Errore di sintassi nel file .asm: %s\n", riga);
            exit(1);
        }

        instr = (instruction*)malloc(sizeof(instruction));
        instr->type = C;
        instr->comp = comp;
        instr->dest = dest;
        instr->jump = jump;
    }

    return instr;
}

void parseC(char* riga, unsigned char* comp, unsigned char* dest, unsigned char* jump) {
    *dest = 0;
    *jump = 0;

    char* compSubString = strchr(riga, '=');
    char* jumpSubString = strchr(riga, ';');

    if (jumpSubString != NULL) {
        jumpSubString[0] = '\0';
        jumpSubString++;
        *jump = cercaValore(jumpSubString, &jumpTable);
    } else {
        *jump = MISSING;
    }

    if (compSubString != NULL) {
        *compSubString = '\0';
        compSubString++;
        *dest = cercaValore(riga, &destTable);
        *comp = cercaValore(compSubString, &compTable);
    } else if (jumpSubString == NULL) {
        *comp = cercaValore(riga, &compTable);
        *dest = MISSING;
    } else {
        *comp = cercaValore(riga, &compTable);
    }
}

int isInstruction(char* riga) {
    return riga[0] == '@' || strchr(riga, '=') || strchr(riga, ';');
}

unsigned char cercaValore(char* chiave, tabella* tabella) {
    for (int i = 0; i < tabella->lunghezza; i++) {
        if (strcmp(chiave, tabella->righe[i].chiave) == 0) {
            return tabella->righe[i].valore;
        }
    }
    return MISSING;
}

void pulisciRiga(char* riga) {
    rimuoviCommento(riga);
    rimuoviSpazi(riga);
    rimuoviNewLine(riga);
}

void rimuoviSpazi(char* string) {
    char* i = string;
    char* j = string;
    while (*j != 0) {
        *i = *j++;
        if (*i != ' ') {
            i++;
        }
    }
    *i = 0;
}

void rimuoviNewLine(char* str) {
    char* ptr = strchr(str, '\n');
    if (ptr) {
        *ptr = '\0';
    }
}

void rimuoviCommento(char* str) {
    char* ptr = strstr(str, "//");
    if (ptr) {
        *ptr = '\0';
    }
}
