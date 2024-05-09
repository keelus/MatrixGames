#ifndef CASILLA_H
#define CASILLA_H

#include "../../colorLED.h"
#include "coordenada.h"
#include "flota.h"
#include <iostream>

namespace flota {
enum class EstadoCasilla {
	AGUA = 0,
	BARCO = 1,
	COLOCANDO = 2,
	ATACANDO = 3,
	HIT_HUNDIDO = 4,
	HIT = 5,
	MISS = 6,
};

class Casilla {
  public:
	Casilla(Coordenada coordenada, int indice, EstadoCasilla estado) : Coord(coordenada), Indice(indice), Estado(estado){};

	Coordenada Coord;
	int Indice;
	EstadoCasilla Estado;
};

std::string EmoticonoCasilla(EstadoCasilla estado);
ColorLED ColorCasilla(EstadoCasilla estado);
std::string Ftemporal(ColorLED color);
} // namespace flota

#endif
