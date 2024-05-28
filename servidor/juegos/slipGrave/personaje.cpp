#include "personaje.h"
#include "../../matrizLED.h"
#include "../../paquetes.h"
#include <string>
#include <unistd.h>

namespace grave {

Personaje::Personaje() { this->SetPosicion(-1, -1); }

void Personaje::SetPosicion(int x, int y) {
	this->x = x;
	this->y = y;
}

int Personaje::interaccion(int *socketUsuario, ColorLED (*mapa)[8][8], MatrizLED *matriz) {
	std::string contenidoPrincipal = "Es hora de moverse!\nPuedes ver el mapa y los obstaculos en la matriz LED.";
	contenidoPrincipal += "\n\nMuevete con W A S y D";

	paquetes::MandarPaquete(*socketUsuario, contenidoPrincipal, "\n\n[ Pulsa una tecla ] ", PULSACION, true);

	paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(*socketUsuario);

	int valor = this->movimiento(paqueteDeCliente.GetContenido()[0], mapa, matriz);

	return valor;
}

int Personaje::movimiento(char tecla, ColorLED (*mapa)[8][8], MatrizLED *matriz) {
	switch (tecla) {
	case 'A':
	case 'a':
		while (true) { // Moverse hacia la izquierda hasta chocar con la pared
			if (this->x != 0) {
				if (matriz->GetPixel(x - 1, y) == ColorLED::Negro) {
					matriz->SetPixel(x, y, ColorLED::Negro);
					matriz->SetPixel(x - 1, y, ColorLED::Verde);

					this->x--;
				} else {
					if (matriz->GetPixel(x - 1, y) == ColorLED::Amarillo) {
						return 1; // Nivel completado

					} else {
						break;
					}
				}
			}
		}
		break;
	case 'D':
	case 'd':
		while (true) { // Moverse hacia la derecha hasta chocar contra la pared
			if (this->x != 7) {
				if (matriz->GetPixel(x + 1, y) == ColorLED::Negro) {
					matriz->SetPixel(x, y, ColorLED::Negro);
					matriz->SetPixel(x + 1, y, ColorLED::Verde);

					this->x++;
				} else {
					if (matriz->GetPixel(x + 1, y) == ColorLED::Amarillo) {
						return 1; // Nivel completado
					} else {
						break;
					}
				}
			}
		}
		break;
	case 'W':
	case 'w':
		while (true) { // Moverse hacia arriba hasta chocar contra la pared
			if (this->y != 0) {
				if (matriz->GetPixel(x, y - 1) == ColorLED::Negro) {
					matriz->SetPixel(x, y, ColorLED::Negro);
					matriz->SetPixel(x, y - 1, ColorLED::Verde);

					this->y--;
				} else {
					if (matriz->GetPixel(x, y - 1) == ColorLED::Amarillo) {
						return 1; // Nivel completado
					} else {
						break;
					}
				}
			}
		}
		break;
	case 'S':
	case 's':
		while (true) { // Moverse hacia abajo hasta chocar contra la pared
			if (this->y != 7) {
				if (matriz->GetPixel(x, y + 1) == ColorLED::Negro) {
					matriz->SetPixel(x, y, ColorLED::Negro);
					matriz->SetPixel(x, y + 1, ColorLED::Verde);

					this->y++;
				} else {
					if (matriz->GetPixel(x, y + 1) == ColorLED::Amarillo) {
						return 1; // Nivel completado
					} else {
						break;
					}
				}
			}
		}
		break;
	}

	return 0; // Nivel no completado
}
} // namespace grave
