#include "mensajes.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tamanyos(char *mensaje, int *tamanyoCodigo, int *tamanyoTextoVisual, int *tamanyoAyudaVisual) {
	int parte = 0;
	for (int i = 0; i < strlen(mensaje); i++) {
		char c = *(mensaje + i);
		if (c == ';') {
			parte++;
			continue;
		}

		switch (parte) {
		case 0:
			(*tamanyoCodigo)++;
			break;
		case 1:
			(*tamanyoTextoVisual)++;
			break;
		case 2:
			(*tamanyoAyudaVisual)++;
			break;
		}
	}
}

Mensaje deserializarMesaje(char *mensaje) {
	Mensaje m = {};

	int tamanyoCodigo = 0;
	int tamanyoTextoVisual = 0;
	int tamanyoAyudaVisual = 0;

	tamanyos(mensaje, &tamanyoCodigo, &tamanyoTextoVisual, &tamanyoAyudaVisual);

	// Deserializar codigo
	char *codigo = malloc(sizeof(char) * (tamanyoCodigo + 1));
	for (int i = 0; i < tamanyoCodigo; i++) {
		*(codigo + i) = *(mensaje + i);
	}
	*(codigo + tamanyoCodigo) = '\0';
	m.codigo = atoi(codigo);

	// Deserializar textoVisual
	m.menu = malloc(sizeof(char) * (tamanyoTextoVisual + 1));
	for (int i = 0; i < tamanyoTextoVisual; i++) {
		*(m.menu + i) = *(mensaje + i + tamanyoCodigo + 1);
	}
	*(m.menu + tamanyoTextoVisual) = '\0';

	// Deserializar ayudaVisual
	m.peticion = malloc(sizeof(char) * (tamanyoAyudaVisual + 1));
	for (int i = 0; i < tamanyoAyudaVisual; i++) {
		*(m.peticion + i) = *(mensaje + i + tamanyoCodigo + 1 + tamanyoTextoVisual + 1);
	}
	*(m.peticion + tamanyoAyudaVisual) = '\0';

	return m;
}

// Primera cifra
// 		2XXX -> OK
// 		3XXX -> Error en logica del servidor (para errores mas inesperados)
// 		9XXX -> Desconectar cliente
// Segunda cifra
// 		X0XX -> Modo texto
// 		X1XX -> Modo pulsaciones de teclas
// Tercera cifra
// 		XX1X -> Limpiar pantalla
// Cuarta cifra -> Para detallar alguna cosa, si es necesario
//
void analizarCodigo(int codigo, bool *cerrarPrograma, bool *limpiarPantalla, ModosEntrada *modoDeEntrada) {
	char *codigoStr = malloc(5 * sizeof(char));
	snprintf(codigoStr, 5 * sizeof(char), "%d", codigo);

	*cerrarPrograma = codigoStr[0] == '9';
	*limpiarPantalla = codigoStr[2] == '1';
	*modoDeEntrada = codigoStr[1] == '0' ? TEXTO : PULSACION;

	// Añadir deteccion de errores (cuando surjan)

	free(codigoStr);
	codigoStr = NULL;
}
