# Assemblatore Nand2Tetris in C

Questo progetto è un'implementazione di un assemblatore per il corso [Nand2Tetris](https://www.nand2tetris.org/) scritto in linguaggio C. L'assemblatore traduce i file di codice assembly di Hack in file binari che possono essere eseguiti sull'Hack Computer, l'architettura del computer sviluppata nel corso Nand2Tetris.

## Struttura del Progetto

- **assembler.c**: Contiene il codice principale dell'assemblatore.
- **parser.c**: Implementa le funzioni per il parsing delle istruzioni assembly.
- **symbolTable.c**: Gestisce la tabella dei simboli per l'assemblatore.
- **assembler.h**: Header file per l'assemblatore.
- **parser.h**: Header file per il parser.
- **symbolTable.h**: Header file per la tabella dei simboli.
- **README.md**: Questo file di documentazione.

## Requisiti

- Un compilatore C (ad esempio, GCC)
- Un editor di testo o un IDE per modificare il codice sorgente

## Installazione

1. Clonare il repository GitHub:

    ```sh
    git clone https://github.com/TechMirko/Assemblatore_Hack.git
    cd Assemblatore_Hack
    ```

2. Compilare il codice sorgente:

    ```sh
    gcc -o assembler assembler.c parser.c symbolTable.c
    ```

## Uso

Eseguire l'assemblatore fornendo come argomento il file di codice assembly `.asm` che si desidera assemblare. Ad esempio:

```sh
./assembler Prog.asm

