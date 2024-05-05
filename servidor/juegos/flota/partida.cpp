#include "partida.h"
#include "ataque.h"
#include "coordenada.h"
#include <exception>
#include <iostream>
#include <string>
#include <synchapi.h>

#include <unistd.h> // Para linux
#include <windows.h>

void flota::Partida::Iteracion() {
	system("cls");

	bool haAcertado = false;

	if (Turno == TiposTurno::TURNO_JUGADOR) {
		std::cout << "Te toca atacar! Tablero de la CPU:";
		TableroCPU.Imprimir(true);

		bool ataqueRealizado = false;
		while (!ataqueRealizado) {
			std::string introducidoSucio;
			std::cout << "Coordenadas de ataque [letra numero]: ";
			std::getline(std::cin, introducidoSucio);

			// TODO: Eliminar espacios
			std::string introducido = introducidoSucio;

			try {
				Coordenada coordenada = ParsearCoordenada(introducido);
				if (TableroCPU.AtaqueYaRecibido(coordenada)) {
					std::cout << "Ataque ya realizado!\n";
				} else {
					ataqueRealizado = true;
					Ataque resultadoAtaque = TableroCPU.RecibirAtaque(coordenada);
					haAcertado = resultadoAtaque.EsHit;

					system("cls");

					if (resultadoAtaque.EsHundido) {
						std::cout << "Tocado y hundido! A la CPU le quedan " << TableroCPU.BarcosRestantes() << " barcos. Tablero de la CPU:";
					} else if (resultadoAtaque.EsHit) {
						std::cout << "Tocado! Tablero de la CPU:";
					} else {
						std::cout << "Agua! Tablero de la CPU:";
					}

					TableroCPU.Imprimir(true);
					sleep(3);
				}

			} catch (char const *e) {
				std::cout << "La coordenada introducida es invalida! Introduce otra:";
			}
		}
	} else {
		std::cout << "Turno de la CPU! Tu tablero:";

		TableroJugador.Imprimir(false);
		sleep(2);
		Ataque resultadoAtaque = TableroJugador.RecibirAtaqueComoIA();
		haAcertado = resultadoAtaque.EsHit;

		system("cls");

		if (resultadoAtaque.EsHundido) {
			std::cout << "Tocado y hundido! Te quedan " << TableroJugador.BarcosRestantes() << " barcos. Tu tablero:";
		} else if (resultadoAtaque.EsHit) {
			std::cout << "Tocado! Tu tablero:";
		} else {
			std::cout << "Agua! Tu tablero:";
		}

		TableroJugador.Imprimir(false);
		sleep(3);
	}

	if (!haAcertado) {
		Turno = Turno == TiposTurno::TURNO_JUGADOR ? TiposTurno::TURNO_CPU : TiposTurno::TURNO_JUGADOR;
	}
}
bool flota::Partida::HaFinalizado() { return TableroJugador.CompletamenteHundido() || TableroCPU.CompletamenteHundido(); }
