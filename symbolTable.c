#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "symbolTable.h"

const int numDefaultSymbols = 23;

void aggiungiSeEtichetta(char *line, symbolTable *st) {
    static unsigned short labelAddr = 0;
    
    if(isInstruction(line)) {
        labelAddr++;
        printf("e' una istruzione: indirizzo ROM: %u\n",labelAddr-1);
        return;
    }
    else printf("non e' una istruzione\n");
    
    // controlliamo sia una dichiarazione di etichetta
    // cioè che cominci e finisca con parentesi
    if(line[0] == '(') {
        if(line[1] != '\0' && !(line[1] <= '9' && line[1] >= '0')) {
            char *end = strchr(line, ')');
            if(end != NULL) {
                *end = '\0';
                line++;

                printf("e' una etichetta: valore: %u\n ",labelAddr);
                aggiungiSimbolo(line, labelAddr, st);
                return;
            }
        }
    } 

    fprintf(stderr, "Errore di sintassi: %s\n", line);
    exit(1);
}

void sostituisciVariable(char *line, symbolTable *st, unsigned short varaddr) {
    
    printf("DEBUG: *line: %s\n", line);
    if(line[0] == '@') 
    {
        printf("DEBUG: sostituisciVariable()\n");
        if(line[1] != '\0' && !(line[1] <= '9' && line[1] >= '0')) 
        {
            unsigned short addr = (unsigned short)cercaInTabella(line+1, st);
            printf("DEBUG: cercaInTabella() = %u\n", addr);
            if(addr == MISSING) {
                addr = varaddr;
                printf("Aggiungo simbolo %s alla tabella dei simboli...\n", line+1);
                aggiungiSimbolo(line+1, varaddr, st);
                printf("Simbolo aggiunto con indirizzo %u\n", varaddr);
                varaddr++;
            }

            //per ottenere una stringa dallo short addr, usiamo sprintf
            sprintf(line+1, "%d", addr);
        }
    }
}

void inizializzaTabella(symbolTable *st) 
{
    // allocazione della tabella ed inizializzazione a 0 con calloc()
    st->symbols = calloc(INITIAL_SIZE, sizeof(symbol));
    st->tableLen = INITIAL_SIZE;
    st->size = 0;

    st->symbols = calloc(INITIAL_SIZE, sizeof(symbol));

    ricopiaTabella(st, defaultSymbols, numDefaultSymbols);
}


void ricopiaTabella(symbolTable *dest, const symbol *src, const size_t srclen) 
{
    int i;
    for(i = 0; i < srclen; i++) {
        if(strlen(src[i].name) != 0) {
            aggiungiSimbolo(src[i].name, src[i].address, dest);
        }
    }
}

unsigned short cercaInTabella(char *name, symbolTable *st) 
{
    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0) {
        if(strcmp(st->symbols[h].name, name) == 0) {
            return st->symbols[h].address; // Simbolo trovato
        }
        h = (h + 1) % st->tableLen; // Corretto incremento
    }
    return MISSING; // Simbolo non trovato
}


void aggiungiSimbolo(const char *name, unsigned short address, symbolTable *st) 
{
    // Controlla se il simbolo esiste già
    unsigned short addr = cercaInTabella(name, st);
    if (addr != MISSING) {
        return; // Simbolo già presente, non aggiungere di nuovo
    }

    int h = hash(name, st->tableLen);
    while(strlen(st->symbols[h].name) != 0)
        h = (h + 1) % st->tableLen;
    
    symbol sym;
    sym.address = address;
    strcpy(sym.name, name);
    
    st->symbols[h] = sym;

    if(st->size++ >= st->tableLen / 2) 
        espandiTabella(st);
}


void espandiTabella(symbolTable *st) 
{
    symbol *tableVals = st->symbols;
    size_t oldLen = st->tableLen;

    /* raddoppia+1 la grandezza della tabella, ricopiandola */
    st->tableLen = (st->tableLen * 2) + 1;
    st->symbols = calloc(st->tableLen, sizeof(symbol));
    st->size = 0;

    ricopiaTabella(st, tableVals, oldLen);

    free(tableVals);
}

size_t hash(const char *key, size_t M) 
{
    /* a,b usate per generare numeri random */
    int h, a = 31415, b = 27183;

    for(h = 0; *key != 0; key++)
        h = (a*h + *key) % M;
    
    return (h < 0) ? (h + M) : h;
}

const symbol defaultSymbols[] = {
      {0, "SP"},  {1, "LCL"},  {2, "ARG"},
    {3, "THIS"}, {4, "THAT"},   {0, "R0"},
      {1, "R1"},   {2, "R2"},   {3, "R3"},
      {4, "R4"},   {5, "R5"},   {6, "R6"},
      {7, "R7"},   {8, "R8"},   {9, "R9"},
    {10, "R10"}, {11, "R11"}, {12, "R12"},
    {13, "R13"}, {14, "R14"}, {15, "R15"},
    {0x4000, "SCREEN"},   {0x6000, "KBD"}
};


