#include "paquetes.h"
#include "sesion.h"
#include "tiposMenu.h"
#include <sys/socket.h>

void paquetes::MandarPaqueteDesconexion(int socketId) {
	Paquete paquete;
	paquete.TextoVisual = "CLOSE";
	paquete.PreInput = " ";
	paquete.Codigo = "9000";
	MandarPaquete(socketId, paquete);
}

void paquetes::MandarPaquete(int socketId, Paquete paquete) {
	std::string paqueteStr = paquete.AString();
	send(socketId, paqueteStr.c_str(), paqueteStr.length(), 0);
	std::cout << "### Mensaje enviado a cliente ###" << std::endl;
}

void paquetes::MandarPaquete(int socketId, std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla) {
	Paquete paquete = {};
	paquete.TextoVisual = textoVisual;
	paquete.PreInput = preInput;

	std::string codigo = "2000";
	codigo[1] = modoEntrada == TEXTO ? '0' : '1';
	codigo[2] = limpiarPantalla ? '1' : '0';

	paquete.Codigo = codigo;

	MandarPaquete(socketId, paquete);
}

void paquetes::MandarPaqueteDeMenu(int socketId, Sesion sesion, std::string mensajeError) {
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
	}

	MandarPaquete(socketId, textoVisual, preInput, modoEntrada, limpiarPantalla);
}
