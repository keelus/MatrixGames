#include "input.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *leerInput(ModosEntrada modo, char *textoVisual) {
	char *buffer = malloc(512 * sizeof(char));
	memset(buffer, 0, 512 * sizeof(char));

	switch (modo) {
	case TEXTO:
		printf("%s", textoVisual);
		fgets(buffer, 512, stdin);
		buffer[strlen(buffer) - 1] = '\0';
		break;
	case PULSACION:
		if (kbhit()) {
			buffer[0] = getch(); // Nota: Las flechas (arriba, abajo, derecha, izquierda) estan mapeadas raras. Mejor seria comprobar en cliente y reemplazar con letras (izquierda = a, derecha = b) etc

			// Si se manda el caracter "Enter", reemplazarlo por "Espacio"
			if (buffer[0] == 10) {
				buffer[0] = ' ';
			}

			buffer[1] = '\0';

			printf("Pulsacion de tecla enviada\n");
		}
		break;
	}

	return buffer;
}
