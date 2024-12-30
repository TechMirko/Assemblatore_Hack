SOURCES = main.c assembler.c parser.c symbolTable.c
OBJECTS = main.o assembler.o parser.o symbolTable.o
EXECUTABLE = assembler

# Variabili per i file di input e output
ASMFILE = input.asm      # File di input
HACKFILE = output.hack  # File di output

# Obiettivo di default
all: $(EXECUTABLE)

# Linker per creare l'eseguibile finale
$(EXECUTABLE): $(OBJECTS)
	gcc -o $(EXECUTABLE) $(OBJECTS)

# Regole per la compilazione dei singoli file oggetto
main.o: main.c
	gcc -c -o main.o main.c

assembler.o: assembler.c
	gcc -c -o assembler.o assembler.c

parser.o: parser.c
	gcc -c -o parser.o parser.c

symbolTable.o: symbolTable.c
	gcc -c -o symbolTable.o symbolTable.c

# Regola per eseguire il programma e passare i file di input e output
run: $(EXECUTABLE)
	./$(EXECUTABLE) $(ASMFILE) $(HACKFILE)

# Pulizia dei file generati
clean:
	rm -f $(EXECUTABLE) *.o *.hack

