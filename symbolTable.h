#define INITIAL_SIZE 60

typedef struct {
    unsigned short address;
    char name[64];
} symbol;

typedef struct {
    size_t tableLen;
    size_t size;
    symbol *symbols; //list di simboli (non array)
} symbolTable;

extern const symbol defaultSymbols[];

void aggiungiSeEtichetta(char *line, symbolTable *st);
void sostituisciVariable(char *line, symbolTable *st, unsigned short varaddr);
void inizializzaTabella(symbolTable *st);
void ricopiaTabella(symbolTable *dest, const symbol *src, const size_t srclen);
unsigned short cercaInTabella(char *symbol, symbolTable *st);
void aggiungiSimbolo(const char *symbol, unsigned short address, symbolTable *st);
void espandiTabella(symbolTable *st);
size_t hash(const char *key, size_t M);

