#include "partida.h"
#include "ataque.h"
#include "coordenada.h"
#include <iostream>
#include <numeric>
#include <string>

#include "../../mensaje.h"
#include "../../paquete.h"

#include <unistd.h> // Para linux

#define TAMANO_BUFFER 1024

bool flota::Partida::Iteracion(int socketId) {
	system("cls");

	bool haAcertado = false;

	if (Turno == TiposTurno::TURNO_JUGADOR) {
		std::string contenidoPrincipal = "Te toca atacar! Tablero de la CPU:";
		contenidoPrincipal += TableroCPU.AString(true);

		mandarPaquete(socketId, contenidoPrincipal, "Introduce una coordenada: ", TEXTO, true);
		std::cout << contenidoPrincipal;

		bool ataqueRealizado = false;
		while (!ataqueRealizado) {
			// std::string introducidoSucio;
			// std::cout << "Coordenadas de ataque [letra numero]: ";
			// std::getline(std::cin, introducidoSucio);

			// TODO: Eliminar espacios

			try {
				MensajeDeCliente mensajeDeCliente = leerDesdeCliente(socketId);
				if (mensajeDeCliente.desconectar)
					return true;

				Coordenada coordenada = ParsearCoordenada(mensajeDeCliente.contenido);
				if (TableroCPU.AtaqueYaRecibido(coordenada)) {
					std::cout << "Ataque ya realizado!\n";
					mandarPaquete(socketId, "", "Ataque ya realizado! Introduce otra coordenada: ", TEXTO, false);
				} else {
					ataqueRealizado = true;
					Ataque resultadoAtaque = TableroCPU.RecibirAtaque(coordenada);
					haAcertado = resultadoAtaque.EsHit;

					system("cls");

					std::string tableroStr = TableroCPU.AString(true);
					std::string resultadoStr;

					if (resultadoAtaque.EsHundido) {
						resultadoStr = "Tocado y hundido! A la CPU le quedan ";
						resultadoStr += TableroCPU.BarcosRestantes();
						resultadoStr += " barcos. Tablero de la CPU:";
					} else if (resultadoAtaque.EsHit) {
						resultadoStr = "Tocado! Tablero de la CPU:";
					} else {
						resultadoStr = "Agua! Tablero de la CPU:";
					}

					std::string contenidoFinal = resultadoStr + tableroStr;

					mandarPaquete(socketId, contenidoFinal, "[ Pulsa una tecla para continuar ]", PULSACION, true);
					std::cout << contenidoFinal << std::endl;

					mensajeDeCliente = leerDesdeCliente(socketId); // Bloquear programa hast recibir mensaje

					if (mensajeDeCliente.desconectar)
						return true;
				}

			} catch (char const *e) {
				std::cout << "Llegamos aqui..." << std::endl;
				std::cout << "La coordenada introducida es invalida! Introduce otra:" << std::endl;
				mandarPaquete(socketId, "", "La coordenada introducida es invalida! Introduce otra: ", TEXTO, false);
			}
		}
	} else {
		// MensajeDeCliente mensajeDeCliente = leerDesdeCliente(socketId);
		// if (mensajeDeCliente.desconectar)
		// 	return true;
		std::string contenidoPrincipal = "Turno de la CPU! Tu tablero:";
		contenidoPrincipal += TableroJugador.AString(false);

		mandarPaquete(socketId, contenidoPrincipal, "[ Pulsa una tecla para continuar ]", PULSACION, true);
		std::cout << contenidoPrincipal;

		MensajeDeCliente mensajeDeCliente = leerDesdeCliente(socketId); // Bloquear hasta continuar
		if (mensajeDeCliente.desconectar)
			return true;

		Ataque resultadoAtaque = TableroJugador.RecibirAtaqueComoIA();
		haAcertado = resultadoAtaque.EsHit;

		std::string tableroStr = TableroJugador.AString(false);
		std::string resultadoStr = "La CPU ha atacado! ";

		if (resultadoAtaque.EsHundido) {
			resultadoStr += "Tocado y hundido! Te quedan ";
			resultadoStr += TableroJugador.BarcosRestantes();
			resultadoStr += " barcos. Tu tablero:";
		} else if (resultadoAtaque.EsHit) {
			resultadoStr += "Tocado! Tu tablero:";
		} else {
			resultadoStr += "Agua! Tu tablero:";
		}

		std::cout << resultadoStr << std::endl;
		std::string contenidoFinal = resultadoStr + tableroStr;

		mandarPaquete(socketId, contenidoFinal, "[ Pulsa una tecla para continuar ]", PULSACION, true);
		std::cout << contenidoFinal << std::endl;

		mensajeDeCliente = leerDesdeCliente(socketId); // Bloquear programa hast recibir mensaje
		if (mensajeDeCliente.desconectar)
			return true;
	}

	if (!haAcertado) {
		Turno = Turno == TiposTurno::TURNO_JUGADOR ? TiposTurno::TURNO_CPU : TiposTurno::TURNO_JUGADOR;
	}

	return false;
}
bool flota::Partida::HaFinalizado() { return TableroJugador.CompletamenteHundido() || TableroCPU.CompletamenteHundido(); }