#include "menu.h"
#include "paquete.h"
#include "tiposMenu.h"
#include <string>

Paquete CrearPaqueteDeMenu(Sesion sesion, std::string mensajeError) {
	Paquete paquete = {};

	switch (sesion.GetMenuActual()) {
	case TiposMenu::Menu0: {
		paquete.TextoVisual = "\nSelecciona una opcion:\n\t1) Iniciar sesion\n\t2) Crear una cuenta\n\n\t3) Salir";
		if (!mensajeError.empty()) {
			paquete.TextoVisual = mensajeError.append(paquete.TextoVisual);
		}

		paquete.PreInput = "\nOpcion seleccionada: ";
		paquete.Codigo = "2010";

		break;
	}
	case TiposMenu::Menu0_Login: {
		if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoUsuario) {
			paquete.TextoVisual = "== INICIAR SESION ==";
			paquete.PreInput = "\nIntroduce tu nombre de usuario: ";
		} else if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoContrasenya) {
			paquete.TextoVisual = "";
			paquete.PreInput = "Introduce tu contrasena: ";
		}
		paquete.Codigo = "2000";

		break;
	}
	case TiposMenu::Menu1: {
		std::string kira = "";
		paquete.TextoVisual = "\n\nHola, ";
		paquete.TextoVisual.append(sesion.GetNombreUsuario());
		paquete.TextoVisual.append("!\n\nSelecciona una opcion:\n\t1) Jugar\n\t2) Configuracion\n\t3) Estadisticas\n\n\t4) Salir");

		paquete.PreInput = "\nElige una de las opciones:  ";
		paquete.Codigo = "2010";

		break;
	}
	case TiposMenu::Menu2: {
		paquete.TextoVisual = "Selecciona un juego:\n\n\t1) Snake\n\t2) Flappy Bird\n\t3) Slip Grave\n\t4) Hundir la flota (vs CPU)\n\t5) 4 en raya (vs CPU)\n\n\t6) Volver atras";

		paquete.PreInput = "\nElige una de las opciones:  ";
		paquete.Codigo = "2010";
		break;
	}
	case TiposMenu::Menu3: {
		paquete.TextoVisual = "Selecciona el juego a configurar:\n\n\t1) Snake\n\t2)Flappy Bird\n\t3) Slip Grave\n\t4) Hundir la flota (vs CPU)\n\t5) 4 en raya (vs CPU)\n\n\t6) Volver atras";

		paquete.PreInput = "\nElige una de las opciones:  ";
		paquete.Codigo = "2010";
		break;
	}

	case TiposMenu::Menu0_Registro: {
		if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoUsuario) {
			paquete.TextoVisual = "== REGISTRO ==";
			paquete.PreInput = "\nIntroduce tu nombre de usuario: ";
		} else if (sesion.GetEstadoLogin() == EstadosLoginRegistro::EsperandoContrasenya) {
			paquete.TextoVisual = "";
			paquete.PreInput = "Introduce tu contrasena: ";
		}

		paquete.Codigo = "2000";
		break;
	}

	case TiposMenu::Cerrar: {
		paquete.TextoVisual = "CLOSE";

		paquete.PreInput = " ";
		paquete.Codigo = "9000";
		break;
	}
	default: {
		printf("Usado un menu no definido. Cuidado.\n");

		paquete.TextoVisual = "El menu actual no esta hecho.";

		paquete.PreInput = "\nCierra el programa.";
		paquete.Codigo = "2000";
		break;
	}
	}

	return paquete;
}
