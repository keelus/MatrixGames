#ifndef TIPOSMENU_H
#define TIPOSMENU_H

enum TiposMenu {
	MENU_0,				// Bienvenida y acciones de login y registro
	MENU_0_LOGIN,		// Inicio de sesion
	MENU_0_REGISTRO,	// Registro de cuenta
	MENU_1,				// Menu principal
	MENU_2,				// Menu de juegos disponibles
	MENU_3,				// Menu configuracion
	MENU_3_JUEGO,		// Menu de configuracion especifico para el juego (se muestran sus posibles colores)
	MENU_3_JUEGO_COLOR, // Donde introduces el color nuevo
	MENU_4,				// Menu estadisticas
	CLOSE,
};

typedef enum TiposMenu TiposMenu;

#endif
