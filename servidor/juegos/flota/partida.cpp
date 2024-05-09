#include "partida.h"
#include "../../matrizLED.h"
#include "ataque.h"
#include "coordenada.h"
#include <iostream>
#include <string>

#include "../../paquetes.h"

#include <unistd.h> // Para linux

#define TAMANO_BUFFER 1024

bool flota::Partida::Iteracion(int socketId, MatrizLED *matrizLED) {
	bool haAcertado = false;

	ColorLED bufferContenido[8][8];

	if (Turno == TiposTurno::TURNO_JUGADOR) {
		TableroJugador.AContenidoColor(bufferContenido, false, true);
		matrizLED->SetMatrizColor(bufferContenido);

		Coordenada coordenadaDeAtaque(0, 0);

		bool ataqueRealizado = false;
		bool errorAtaqueYaRealizao = false;

		while (!ataqueRealizado) {
			std::string contenidoPrincipal = "Te toca atacar!\nPuedes ver el tablero de tu rival, y tus ataques en la matriz LED.";
			contenidoPrincipal += "\n\nMueve el punto de ataque con W A S y D.\nUsa la tecla Enter para atacar!";
			contenidoPrincipal += "\n\nCoordenada de ataque actual -> (Fila: " + std::to_string(coordenadaDeAtaque.Y) + ", Columna: " + std::to_string(coordenadaDeAtaque.X) + ")";

			if (errorAtaqueYaRealizao)
				contenidoPrincipal += "\n\nYa has realizado ese ataque!";

			paquetes::MandarPaquete(socketId, contenidoPrincipal, "\n\n[ Pulsa una tecla ] ", PULSACION, true);
			std::cout << contenidoPrincipal;

			paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId);
			if (paqueteDeCliente.SeQuiereDesconectar())
				return true;

			switch (paqueteDeCliente.GetContenido()[0]) {
			case 'A':
			case 'a':
				if (coordenadaDeAtaque.X > 0)
					coordenadaDeAtaque.X--;
				break;
			case 'D':
			case 'd':
				if (coordenadaDeAtaque.X < 7)
					coordenadaDeAtaque.X++;
				break;
			case 'W':
			case 'w':
				if (coordenadaDeAtaque.Y > 0)
					coordenadaDeAtaque.Y--;
				break;
			case 'S':
			case 's':
				if (coordenadaDeAtaque.Y < 7)
					coordenadaDeAtaque.Y++;
				break;
			case ' ':
				if (TableroCPU.AtaqueYaRecibido(coordenadaDeAtaque)) {
					errorAtaqueYaRealizao = true;
					std::cout << "Ataque ya realizado!\n";
				} else {
					ataqueRealizado = true;
					Ataque resultadoAtaque = TableroCPU.RecibirAtaque(coordenadaDeAtaque);
					haAcertado = resultadoAtaque.EsHit;

					std::string tableroStr = TableroCPU.AString(true, false);
					std::string resultadoStr;

					if (resultadoAtaque.EsHundido) {
						resultadoStr = "Tocado y hundido! A la CPU le quedan ";
						resultadoStr += std::to_string(TableroCPU.BarcosRestantes());
						resultadoStr += " barcos. \nPuedes ver el tablero de la CPU actual en la matriz LED.";
					} else if (resultadoAtaque.EsHit) {
						resultadoStr = "Tocado! \nPuedes ver el tablero de la CPU actual en la matriz LED.";
					} else {
						resultadoStr = "Agua! \nPuedes ver el tablero de la CPU actual en la matriz LED.";
					}

					std::string contenidoFinal = resultadoStr /* + tableroStr*/;

					TableroCPU.AContenidoColor(bufferContenido, true, false);
					matrizLED->SetMatrizColor(bufferContenido);

					paquetes::MandarPaquete(socketId, contenidoFinal, "\n\n[ Pulsa una tecla para continuar ]", PULSACION, true);
					std::cout << contenidoFinal << std::endl;

					paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId); // Bloquear programa hast recibir mensaje

					if (paqueteDeCliente.SeQuiereDesconectar())
						return true;
				}
				break;
			}
		}
	} else {
		// MensajeDeCliente mensajeDeCliente = leerDesdeCliente(socketId);
		// if (mensajeDeCliente.desconectar)
		// 	return true;
		std::string contenidoPrincipal = "Turno de la CPU!\nPuedes ver tu tablero y los ataques del rival en la matriz LED.";
		// contenidoPrincipal += TableroJugador.AString(false);

		TableroJugador.AContenidoColor(bufferContenido, false, false);
		matrizLED->SetMatrizColor(bufferContenido);

		paquetes::MandarPaquete(socketId, contenidoPrincipal, "\n\n[ Pulsa una tecla para continuar ]", PULSACION, true);
		std::cout << contenidoPrincipal;

		paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId); // Bloquear hasta continuar
		if (paqueteDeCliente.SeQuiereDesconectar())
			return true;

		Ataque resultadoAtaque = TableroJugador.RecibirAtaqueComoIA();
		haAcertado = resultadoAtaque.EsHit;

		std::string tableroStr = TableroJugador.AString(false, false);
		std::string resultadoStr = "La CPU ha atacado! ";

		if (resultadoAtaque.EsHundido) {
			resultadoStr += "Tocado y hundido!\nTe quedan ";
			resultadoStr += std::to_string(TableroJugador.BarcosRestantes());
			resultadoStr += " barcos.\nPuedes ver tu tablero y los ataques del rival en la matriz LED.";
		} else if (resultadoAtaque.EsHit) {
			resultadoStr += "Tocado!\nPuedes ver tu tablero y los ataques del rival en la matriz LED.";
		} else {
			resultadoStr += "Agua!\nPuedes ver tu tablero y los ataques del rival en la matriz LED.";
		}

		std::cout << resultadoStr << std::endl;
		std::string contenidoFinal = resultadoStr /* + tableroStr*/;

		TableroJugador.AContenidoColor(bufferContenido, false, false);

		matrizLED->SetMatrizColor(bufferContenido);

		paquetes::MandarPaquete(socketId, contenidoFinal, "\n\n[ Pulsa una tecla para continuar ]", PULSACION, true);
		std::cout << contenidoFinal << std::endl;

		paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId); // Bloquear programa hast recibir mensaje
		if (paqueteDeCliente.SeQuiereDesconectar())
			return true;
	}

	if (!haAcertado) {
		Turno = Turno == TiposTurno::TURNO_JUGADOR ? TiposTurno::TURNO_CPU : TiposTurno::TURNO_JUGADOR;
	}

	return false;
}
bool flota::Partida::HaFinalizado() { return TableroJugador.CompletamenteHundido() || TableroCPU.CompletamenteHundido(); }
