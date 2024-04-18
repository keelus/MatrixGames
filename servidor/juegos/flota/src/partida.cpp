#include "partida.h"
#include "ataque.h"
#include "coordenada.h"
#include <exception>
#include <iostream>
#include <string>
#include <synchapi.h>

#include <unistd.h> // Para linux
#include <windows.h>

void Partida::Iteracion() {
	system("cls");

	bool haAcertado = false;

	if (Turno == TiposTurno::TURNO_JUGADOR) {
		std::cout << "Te toca atacar! Tablero de la CPU:";
		TableroCPU.Imprimir(true);
		std::cout << "Coordenadas de ataque [letra numero]: ";

		bool ataqueRealizado = false;

		while (!ataqueRealizado) {
			// Leer input

			std::string introducidoSucio;
			std::getline(std::cin, introducidoSucio);

			// TODO: Eliminar espacios
			std::string introducido = introducidoSucio;

			try {
				Coordenada coordenada = ParsearCoordenada(introducido);
				std::cout << "Coordenada: {X: " << coordenada.X << ", Y: " << coordenada.Y << "}";
				if (TableroCPU.AtaqueYaRecibido(coordenada)) {
					std::cout << "Ataque ya realizado!";
				} else {
					ataqueRealizado = true;
					Ataque resultadoAtaque = TableroCPU.RecibirAtaque(coordenada);
					bool haAcertado = resultadoAtaque.EsHit;

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
	}
}
bool Partida::HaFinalizado() { return TableroJugador.CompletamenteHundido() || TableroCPU.CompletamenteHundido(); }
