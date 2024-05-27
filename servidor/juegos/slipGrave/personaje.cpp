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

int Personaje::interaccion(int *socketUsuario, ColorLED (*mapa)[8][8], MatrizLED *matriz) {

	std::string contenidoPrincipal = "Es hora de moverse!\nPuedes ver el mapa y los obstaculos en la matriz LED.";
	contenidoPrincipal += "\n\nMuevete con W A S y D";
	std::cout << "mandando paquete" << std::endl;
	paquetes::MandarPaquete(*socketUsuario, contenidoPrincipal, "\n\n[ Pulsa una tecla ] ", PULSACION, true);
	std::cout << "mandado" << std::endl;
	std::cout << contenidoPrincipal << std::endl;
	std::cout << "esperando desde cliente" << std::endl;
	paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(*socketUsuario);
	std::cout << "recibido" << std::endl;
	int valor = this->movimiento(paqueteDeCliente.GetContenido()[0], mapa ,matriz);
	return valor;
}
int Personaje::movimiento(char tecla, ColorLED (*mapa)[8][8], MatrizLED* matriz) {
	
	int i = 0;
	printf("prueba");
	switch (tecla) {
	case 'A':
	case 'a':
		i = 0;
		while (i == 0) {
			if (this->x != 0) {
				
				if (matriz->GetPixel(x-1,y) == ColorLED::Negro) {
					matriz->SetPixel(x,y,ColorLED::Negro);
					matriz->SetPixel(x-1,y,ColorLED::Verde);
					x = x - 1;
				} else {
					if (matriz->GetPixel(x-1,y) == ColorLED::Amarillo) {
						// nivel pasado
						return 1 ;

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
		while (i == 0) {
			
			if (this->x != 7) {
				printf("Color en hexadecimal: y es %d e x es %d y tmbn %#010X\n",y,this->x,ColorLED::Verde);
				if (matriz->GetPixel(x+1,y) == ColorLED::Negro) {
					matriz->SetPixel(x,y, ColorLED::Negro);
					matriz->SetPixel(x+1,y, ColorLED::Verde);
					this->x = this->x + 1;

				} else {
					if (matriz->GetPixel(x+1,y) == ColorLED::Amarillo) {
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
		while (i == 0) {
			if (this->y != 0) {
				if (matriz->GetPixel(x,y-1) == ColorLED::Negro) {
					matriz->SetPixel(x,y, ColorLED::Negro);
					matriz->SetPixel(x,y-1, ColorLED::Verde);
					this->y = this->y - 1;
					

				} else {
					if (matriz->GetPixel(x,y-1) == ColorLED::Amarillo) {
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
		while (i == 0) {
			if (this->y != 7) {
				if (matriz->GetPixel(x,y+1) == ColorLED::Negro) {
					matriz->SetPixel(x,y, ColorLED::Negro);
					matriz->SetPixel(x,y+1, ColorLED::Verde);
					this->y = this->y + 1;
					

				} else {
					if (matriz->GetPixel(x,y+1) == ColorLED::Amarillo) {
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
	
	return 0;
	
}
// Destructor
Personaje::~Personaje() {
	// Código de limpieza si es necesario
}

} // namespace grave
