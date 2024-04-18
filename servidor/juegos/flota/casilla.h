#ifndef CASILLA_H
#define CASILLA_H

#include "coordenada.h"
#include <iostream>

enum class EstadoCasilla {
	AGUA = 0,
	BARCO = 1,
	COLOCANDO = 2,
	HIT_HUNDIDO = 3,
	HIT = 4,
	MISS = 5,
};

class Casilla {
  public:
	Casilla(Coordenada coordenada, int indice, EstadoCasilla estado) : Coord(coordenada), Indice(indice), Estado(estado){};

	Coordenada Coord;
	int Indice;
	EstadoCasilla Estado;
};

std::string EmoticonoCasilla(EstadoCasilla estado);

#endif
