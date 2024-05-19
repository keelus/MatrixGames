#include "personaje.h"
#include "../../matrizLED.h"
#include "../../paquetes.h"
#include "partida.h"
#include <iostream>
#include <string>
#include <unistd.h> // Para linux
namespace grave {
// Constructor
Personaje::Personaje() {
	// Código de inicialización si es necesario
}

int Personaje::interaccion(int socketUsuario, ColorLED mapa[8][8]) {

	std::string contenidoPrincipal = "Es hora de moverse!\nPuedes ver el mapa y los obstaculos en la matriz LED.";
	contenidoPrincipal += "\n\nMuevete con W A S y D";
	std::cout << "mandando paquete" << std::endl;
	paquetes::MandarPaquete(socketUsuario, contenidoPrincipal, "\n\n[ Pulsa una tecla ] ", PULSACION, true);
	std::cout << "mandado" << std::endl;
	std::cout << contenidoPrincipal << std::endl;
	std::cout << "esperando desde cliente" << std::endl;
	paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketUsuario);
	std::cout << "recibido" << std::endl;
	int valor = this->movimiento(paqueteDeCliente.GetContenido()[0], mapa);
	return valor;
}
int Personaje::movimiento(char tecla, ColorLED mapa[8][8]) {
	printf("6");
	int i = 0;
	switch (tecla) {
	case 'A':
	case 'a':
		i = 0;
		while (i == 1) {
			if (this->x != 0) {
				if (mapa[y][x - 1] == ColorLED::Negro) {
					mapa[y][x] = ColorLED::Negro;
					mapa[y][x - 1] = ColorLED::Verde;
					this->x = x - 1;
				} else {
					if (mapa[y][x - 1] == ColorLED::Amarillo) {
						// nivel pasado
						return 1;

					} else {
						i = 1;
					}
				}
			}
		}
		break;
	case 'D':
	case 'd':
		i = 0;
		while (i == 1) {
			if (this->x != 7) {
				if (mapa[y][x + 1] == ColorLED::Negro) {
					mapa[y][x] = ColorLED::Negro;
					mapa[y][x + 1] = ColorLED::Verde;
					this->x = x + 1;
				} else {
					if (mapa[y][x + 1] == ColorLED::Amarillo) {
						// nivel pasado
						return 1;
					} else {
						i = 1;
					}
				}
			}
		}
		break;
	case 'W':
	case 'w':
		i = 0;
		while (i == 1) {
			if (this->y != 0) {
				if (mapa[y - 1][x] == ColorLED::Negro) {
					mapa[y][x] = ColorLED::Negro;
					mapa[y - 1][x] = ColorLED::Verde;
					this->y = y - 1;
				} else {
					if (mapa[y - 1][x] == ColorLED::Amarillo) {
						// nivel pasado
						return 1;
					} else {
						i = 1;
					}
				}
			}
		}
		break;
	case 'S':
	case 's':
		i = 0;
		while (i == 1) {
			if (this->y != 7) {
				if (mapa[y + 1][x] == ColorLED::Negro) {
					mapa[y][x] = ColorLED::Negro;
					mapa[y + 1][x] = ColorLED::Verde;
					this->y = y + 1;
				} else {
					if (mapa[y + 1][x] == ColorLED::Amarillo) {
						// nivel pasado
						return 1;
					} else {
						i = 1;
					}
				}
			}
		}
		break;
	}
}
// Destructor
Personaje::~Personaje() {
	// Código de limpieza si es necesario
}

} // namespace grave
