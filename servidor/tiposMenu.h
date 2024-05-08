#ifndef TIPOSMENU_H
#define TIPOSMENU_H

enum class EstadosLoginRegistro { EsperandoUsuario, EsperandoContrasenya };

enum class TiposMenu {
	Menu0,			// Bienvenida y acciones de login y registro
	Menu0_Login,	// Inicio de sesion
	Menu0_Registro, // Registro de cuenta
	Menu1,			// Menu principal
	Menu2,			// Menu de juegos disponibles
	Menu3,
	Menu3_Juego,
	Menu3_JuegoColor,
	Menu4, // Menu estadisticas
	Cerrar
};

#endif
