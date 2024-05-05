#include "input.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_BUFFER 1024

char *leerInput(ModosEntrada modo, char *textoVisual) {
	char *buffer = malloc(TAMANO_BUFFER * sizeof(char));
	memset(buffer, 0, TAMANO_BUFFER * sizeof(char));

	printf("%s", textoVisual);
	switch (modo) {
	case TEXTO:
		fgets(buffer, TAMANO_BUFFER, stdin);
		if (strlen(buffer) == 0)
			buffer[0] = ' '; // Añadir espacio, para prevenir un mensaje vacio que el servidor ignorará. Esto haría que los mensajes y visuales irían desincronizaods."
		buffer[strlen(buffer) - 1] = '\0';
		break;
	case PULSACION:
		while (1) {
			if (kbhit()) {
				buffer[0] = getch(); // Nota: Las flechas (arriba, abajo, derecha, izquierda) estan mapeadas raras. Mejor seria comprobar en cliente y reemplazar con letras (izquierda = a, derecha = b) etc

				// Si se manda la tecla enter, reemplazarla por un espacio, para prevenir un mensaje vacio que el servidor ignorará. Esto haría que los mensajes y visuales irían desincronizaods."
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
