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
		std::string contenidoPrincipal = "Te toca atacar!\nPuedes ver el tablero de tu rival, y tus ataques en la matriz LED.";
		// contenidoPrincipal += TableroCPU.AString(true);

		TableroCPU.AContenidoColor(bufferContenido, true);
		matrizLED->SetMatrizColor(bufferContenido);

		paquetes::MandarPaquete(socketId, contenidoPrincipal, "\nIntroduce la coordenada de ataque (ejemplo: d4): ", TEXTO, true);
		std::cout << contenidoPrincipal;

		bool ataqueRealizado = false;
		while (!ataqueRealizado) {
			try {
				paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId);
				if (paqueteDeCliente.SeQuiereDesconectar())
					return true;

				// TODO: Eliminar espacios desde el input

				Coordenada coordenada = ParsearCoordenada(paqueteDeCliente.GetContenido());
				if (TableroCPU.AtaqueYaRecibido(coordenada)) {
					std::cout << "Ataque ya realizado!\n";
					paquetes::MandarPaquete(socketId, "Ya has realizado ese ataque!", "\nIntroduce otra coordenada (ejemplo: d4): ", TEXTO, false);
				} else {
					ataqueRealizado = true;
					Ataque resultadoAtaque = TableroCPU.RecibirAtaque(coordenada);
					haAcertado = resultadoAtaque.EsHit;

					std::string tableroStr = TableroCPU.AString(true);
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

					TableroCPU.AContenidoColor(bufferContenido, true);
					matrizLED->SetMatrizColor(bufferContenido);

					paquetes::MandarPaquete(socketId, contenidoFinal, "\n[ Pulsa una tecla para continuar ]", PULSACION, true);
					std::cout << contenidoFinal << std::endl;

					paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId); // Bloquear programa hast recibir mensaje

					if (paqueteDeCliente.SeQuiereDesconectar())
						return true;
				}

			} catch (char const *e) {
				paquetes::MandarPaquete(socketId, "", "La coordenada introducida es invalida! \n Introduce otra coordenada (ejemplo: d4): ", TEXTO, false);
			}
		}
	} else {
		// MensajeDeCliente mensajeDeCliente = leerDesdeCliente(socketId);
		// if (mensajeDeCliente.desconectar)
		// 	return true;
		std::string contenidoPrincipal = "Turno de la CPU!\nPuedes ver tu tablero y los ataques del rival en la matriz LED.";
		// contenidoPrincipal += TableroJugador.AString(false);

		TableroJugador.AContenidoColor(bufferContenido, false);
		matrizLED->SetMatrizColor(bufferContenido);

		paquetes::MandarPaquete(socketId, contenidoPrincipal, "\n[ Pulsa una tecla para continuar ]", PULSACION, true);
		std::cout << contenidoPrincipal;

		paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketId); // Bloquear hasta continuar
		if (paqueteDeCliente.SeQuiereDesconectar())
			return true;

		Ataque resultadoAtaque = TableroJugador.RecibirAtaqueComoIA();
		haAcertado = resultadoAtaque.EsHit;

		std::string tableroStr = TableroJugador.AString(false);
		std::string resultadoStr = "La CPU ha atacado!";

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

		TableroJugador.AContenidoColor(bufferContenido, false);

		matrizLED->SetMatrizColor(bufferContenido);

		paquetes::MandarPaquete(socketId, contenidoFinal, "\n[ Pulsa una tecla para continuar ]", PULSACION, true);
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
