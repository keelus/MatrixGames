#include "paquetes.h"
#include "sesion.h"
#include "tiposMenu.h"
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TAMANO_BUFFER 1024

// Paquetes de cliente
paquetes::PaqueteDeCliente::PaqueteDeCliente(std::string contenido, bool quiereDesconectarse) {
	this->contenido = contenido;
	this->desconectar = quiereDesconectarse;
}

std::string paquetes::PaqueteDeCliente::GetContenido() { return this->contenido; }

bool paquetes::PaqueteDeCliente::SeQuiereDesconectar() { return this->desconectar; }

paquetes::PaqueteDeCliente paquetes::LeerPaqueteDesdeCliente(int socketId) {
	ssize_t valread;
	char buffer[TAMANO_BUFFER] = {0};

	while (true) {
		valread = read(socketId, buffer, TAMANO_BUFFER - 1);

		if (valread == 0) { // Desconexion
			return PaqueteDeCliente("", true);
		}

		if (buffer[0] != '\0' && buffer[0] != '\\') { // Es valido
			std::cout << "> Recibido desde cliente: \"" << buffer << "\" [tamano: " << valread << ", " << strlen(buffer) << "]" << std::endl;

			return PaqueteDeCliente(buffer, false);
		} else {
			std::cout << "\t> Ignorado: \"" << buffer << "\" [tamano: " << valread << ", " << strlen(buffer) << "]" << std::endl;
		}
	}
}

// Paquetes de servidor

paquetes::PaqueteDeServidor::PaqueteDeServidor(std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla) {
	this->textoVisual = textoVisual;
	this->preInput = preInput;

	this->codigo = "2000";
	this->codigo[1] = modoEntrada == TEXTO ? '0' : '1';
	this->codigo[2] = limpiarPantalla ? '1' : '0';
}

paquetes::PaqueteDeServidor::PaqueteDeServidor(std::string textoVisual, std::string preInput, std::string codigo) {
	this->textoVisual = textoVisual;
	this->preInput = preInput;
	this->codigo = codigo;
}

std::string paquetes::PaqueteDeServidor::AString() {
	std::string stringFinal = this->codigo.append(";").append(this->textoVisual).append(";").append(this->preInput).append(";");
	return stringFinal;
}

void paquetes::MandarPaquete(int socketId, PaqueteDeServidor paquete) {
	std::string paqueteStr = paquete.AString();
	send(socketId, paqueteStr.c_str(), paqueteStr.length(), 0);
	std::cout << "### Mensaje enviado a cliente ###" << std::endl;
}

void paquetes::MandarPaqueteDesconexion(int socketId) {
	PaqueteDeServidor paquete("CLOSE", " ", "9000");
	MandarPaquete(socketId, paquete);
}

void paquetes::MandarPaquete(int socketId, std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla) {
	PaqueteDeServidor paquete(textoVisual, preInput, modoEntrada, limpiarPantalla);
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
