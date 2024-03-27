#ifndef _MENUH_
#define _MENUH_

#include "sql.h"

enum TiposMenu {
	MENU_0,		    // Bienvenida y acciones de login y registro
	MENU_0_LOGIN,	    // Inicio de sesion
	MENU_0_REGISTRO,    // Registro de cuenta
	MENU_1,		    // Menu principal
	MENU_2,		    // Menu de juegos disponibles
	MENU_3,		    // Estadisticas
	MENU_4,		    // Menu configuracion
	MENU_4_JUEGO,	    // Menu de configuracion especifico para el juego (se muestran sus posibles colores)
	MENU_4_JUEGO_COLOR, // Donde introduces el color nuevo
};
typedef enum TiposMenu TiposMenu;

struct Menu {
	const char *TextoVisual;
	const char *PreInput;
	const char *Codigo;
};
typedef struct Menu Menu;

Menu mensajeMenu(TiposMenu menuActual, EstadosMenuLogin estadoLogin, const char *mensajeError, const char *usuario, const char *juegoSeleccionadoConf);

const char *menuAString(Menu menu);

#endif
