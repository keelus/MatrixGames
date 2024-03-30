#ifndef _MENSAJESH_
#define _MENSAJESH_

#include "input.h"
#include <stdbool.h>

struct Mensaje {
	int codigo;
	char *menu;
	char *peticion;
};
typedef struct Mensaje Mensaje;

Mensaje deserializarMesaje(char *mensaje);
void analizarCodigo(int codigo, bool *cerrarPrograma, bool *limpiarPantalla, ModosEntrada *modoDeEntrada);

#endif
