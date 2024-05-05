#include "input.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_BUFFER 512

char *leerInput(ModosEntrada modo, char *textoVisual) {
	char *buffer = malloc(*sizeof(char));
	memset(buffer, 0, TAMANO_BUFFER * sizeof(char));

	printf("%s", textoVisual);
	switch (modo) {
	case TEXTO:
		fgets(buffer, TAMANO_BUFFER, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		break;
	case PULSACION:
		while (1) {
			if (kbhit()) {
				buffer[0] = getch(); // Nota: Las flechas (arriba, abajo, derecha, izquierda) estan mapeadas raras. Mejor seria comprobar en cliente y reemplazar con letras (izquierda = a, derecha = b) etc

				// Si se manda el caracter "Enter", reemplazarlo por "Espacio"
				if (buffer[0] == 10) {
					buffer[0] = ' ';
				}

				buffer[1] = '\0';

				break;
			}
		}
		break;
	}

	return buffer;
}
