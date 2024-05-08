#include "menu.h"
#include "paquete.h"
#include "tiposMenu.h"
#include <string>
#include <sys/socket.h>

void CrearYMandarPaqueteDeMenu(int socketId, Sesion sesion, std::string mensajeError) {
	std::string textoVisual, preInput;
	ModosEntrada modoEntrada;
	bool limpiarPantalla;

	switch (sesion.GetMenuActual()) {
	case TiposMenu::Menu0: {
		textoVisual = "\nSelecciona una opcion:\n\t1) Iniciar sesion\n\t2) Crear una cuenta\n\n\t3) Salir";
		if (!mensajeError.empty()) {
			textoVisual = mensajeError.append(textoVisual);
		}

		preInput = "\nOpcion seleccionada: ";
		limpiarPantalla = true;
		modoEntrada = TEXTO;

		break;
	}
	case TiposMenu::Menu0_Login: {
		if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoUsuario) {
			textoVisual = "== INICIAR SESION ==";
			preInput = "\nIntroduce tu nombre de usuario: ";
		} else if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoContrasenya) {
			textoVisual = "";
			preInput = "Introduce tu contrasena: ";
		}

		limpiarPantalla = false;
		modoEntrada = TEXTO;

		break;
	}
	case TiposMenu::Menu0_Registro: {
		if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoUsuario) {
			textoVisual = "== REGISTRO ==";
			preInput = "\nIntroduce tu nombre de usuario: ";
		} else if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoContrasenya) {
			textoVisual = "";
			preInput = "Introduce tu contrasena: ";
		}

		limpiarPantalla = false;
		modoEntrada = TEXTO;

		break;
	}
	case TiposMenu::Menu1: {
		textoVisual = "Hola, ";
		textoVisual += sesion.GetNombreUsuario();
		textoVisual.append("!\n\nSelecciona una opcion:\n\t1) Jugar\n\t2) Estadisticas\n\n\t3) Salir");

		preInput = "\nElige una de las opciones:  ";

		limpiarPantalla = true;
		modoEntrada = TEXTO;

		break;
	}

	case TiposMenu::Menu2: {
		textoVisual = "Selecciona un juego:\n\n\t1) Snake\n\t2) Flappy Bird\n\t3) Slip Grave\n\t4) Hundir la flota (vs CPU)\n\t5) 4 en raya (vs CPU)\n\n\t6) Volver atras";

		preInput = "\nElige una de las opciones:  ";

		limpiarPantalla = true;
		modoEntrada = TEXTO;

		break;
	}

	case TiposMenu::Menu3: {
		textoVisual = "Aqui irian tus estadisticas...";
		preInput = "\n[ Pulsa cualquier tecla para volver al menu principal ]";

		limpiarPantalla = true;
		modoEntrada = PULSACION;

		break;
	}

	case TiposMenu::Cerrar: {
		textoVisual = "CLOSE";

		preInput = " ";

		limpiarPantalla = true;
		modoEntrada = TEXTO;

		break;
	}
	default: {
		textoVisual = "El menu actual no esta hecho.";

		preInput = "\nCierra el programa.";

		limpiarPantalla = true;
		modoEntrada = TEXTO;

		break;
	}
	}

	if (textoVisual == "CLOSE") {
		Paquete paqueteDesconexion;
		paqueteDesconexion.Codigo = "9000";
		paqueteDesconexion.TextoVisual = textoVisual;
		paqueteDesconexion.PreInput = preInput;
		mandarPaquete(socketId, paqueteDesconexion);
	} else {
		mandarPaquete(socketId, textoVisual, preInput, modoEntrada, limpiarPantalla);
	}
}
