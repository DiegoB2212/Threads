all: Compilar Run Erase

Compilar: main.c
	gcc main.c -o Compilador -pthread

Clean:
	clear

Erase: Compilador
	rm Compilador

Run: Compilador
	./Compilador
