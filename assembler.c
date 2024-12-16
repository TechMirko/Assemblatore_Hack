#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"
#include "parser.h"
#include "symbolTable.h"

// prototipo
void traduciInBinario(unsigned short data, char *binarystring);

// legge il file ed assembla
void assemble(FILE *asmfile, FILE *hackfile) 
{
    char buf[256]; // max 256 chars per riga dal file
    
    symbolTable st;
    inizializzaTabella(&st);

    // prima passata   
    while (fgets(buf, 256, asmfile) != NULL) 
    {
        // stringa letta ha già il newline (a parte forse ultima)
        printf("-----\nho letto: %s", buf); 
        
        // aggiorna buf: rimuove spazi, newline, commenti
        pulisciRiga(buf);

        if (buf[0] == '\0') {
            printf("stringa vuota: continuo\n");
            continue;
        }
        
        printf("pulita: %s\n", buf);
        
        // aggiunge etichetta-valore in tabella, se trova una etichetta
        aggiungiSeEtichetta(buf, &st);
    }

    // riposiziona il puntatore all'inizio del file
    rewind(asmfile);
    printf("\n==========\nseconda passata\n==========\n\n"); 
            
    // data sarà il valore dell'istruzione da tradurre in binario
    unsigned short data;
    // stringa in binario
    char binarystring[18];
    unsigned short varaddr = 16;
    // seconda passata
    while (fgets(buf, 256, asmfile) != NULL) 
    {
        printf("-----\nho letto: %s", buf); 
        
        pulisciRiga(buf);
        printf("Riga pulita %s\n", buf);
        // se si tratta di una A-instruction, sostituisce il valore
        if (buf[0] == '@') {
            char *symbol = buf + 1;
            // Convert the symbol to its address
            if (isalpha(symbol[0])) {  
                // Symbol is a variable or label
                data = (unsigned short)cercaInTabella(symbol, &st);
                printf("DEBUG: data = %u\n", data); // agiunge troppe variabili
                if (data == MISSING) {
                    printf("Variabile nuova, la aggiungo\n");
                    sostituisciVariable(buf, &st, varaddr);
                    data = varaddr;
                    varaddr++;
                }
            } else if (!isalpha(symbol[0])) {
                // Symbol is a constant value
                data = (unsigned short)atoi(symbol);
            }  
            printf("A-instruction: valore %u\n", data);
        } else if (!isInstruction(buf)) {
            printf("Non e' un'istruzione: continuo\n");
            continue;
        } else {
            // parse and return a pointer
            instruction* op = parseInstruction(buf);

            if (op == NULL) {
                printf("Errore nel parsing dell'istruzione: %s\n", buf);
                continue;
            }

            if (op->type == A) {
                data = op->valore;
            } else {
                // Instead of taking binary encodings of dest, comp, jump
                // and concatenating them as strings, use shift.

                // Format C-instructions: 111 a c1-c6 d1-d3 j1-j3
                // quindi 7 shiftato di 13 + comp << 6 + dest << 3 + jump 
                data = (0x7 << 13) + (op->comp << 6) + (op->dest << 3) + (op->jump);
            }

            free(op); // Don't forget to free the allocated instruction
        }

        printf("valore: %u\n", data);

        // binarystring sarà il valore in binario (come stringa)
        traduciInBinario(data, binarystring);
        printf("Traduzione in binario: %s\n", binarystring);
        fprintf(hackfile, "%s", binarystring);
    }
}

void traduciInBinario(unsigned short data, char *binarystring)
{    
    // shift a destra di 15-j per selezionare prima il bit più significativo
    // poi i 2 bit più significativi, poi i 3, etc. Calcolando "... & 1", 
    // controlliamo se il j-esimo bit (dal *più* significativo) sia 1 o 0:
    // corrisponde a calcolare data[15]&1, data[15..14]&01, data[15..13]&001, ...

    int j;
    for (j = 0; j < 16; j++) {
        binarystring[j] = (data >> (15-j)) & 1 ? '1' : '0';
    }

    binarystring[16] = '\n';
    binarystring[17] = '\0';

    printf("in binario: %s", binarystring);
}
