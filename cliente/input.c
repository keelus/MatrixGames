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

		// Si mensaje vacio, o mensaje vacio + enter ( TODO: Prevenir guardar caracter enter )
		if (strlen(buffer) == 0 || strlen(buffer) == 1) {
			// Añadir espacio, para prevenir un mensaje vacio que el servidor ignorara.
			// Esto haría que los mensajes y visuales irian desincronizados."
			buffer[0] = ' ';
			buffer[1] = '\0';
		} else {
			// Si no, simplemente añadimos caracter nulo al final
			buffer[strlen(buffer) - 1] = '\0';
		}

		// Finalmente, evitar que el mensaje empiece por \, pues causaria lo ya mencionado antes.
		if (buffer[0] == '\\') {
			buffer[0] = ' ';
		}

		break;
	case PULSACION:
		while (1) {
			if (kbhit()) {
				buffer[0] = getch();

				// Prevenir caracter enter (vacio)
				if (buffer[0] == 10 || buffer[0] == 13) {
					buffer[0] = ' ';
				}

				if (buffer[0] == '\\') {
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
