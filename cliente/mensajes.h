#ifndef _MENSAJESH_
#define _MENSAJESH_

struct Mensaje {
	int codigo;
	char *menu;
	char *peticion;
};
typedef struct Mensaje Mensaje;

Mensaje deserializarMesaje(char *mensaje);

#endif
