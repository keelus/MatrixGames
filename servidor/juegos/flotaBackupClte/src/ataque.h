#ifndef ATAQUE_H
#define ATAQUE_H

#include "coordenada.h"

class Ataque {
  public:
	Ataque(bool esHit, bool esHundido, Coordenada coord) : EsHit(esHit), EsHundido(esHundido), Coord(coord){};
	bool EsHit, EsHundido;
	Coordenada Coord;
};

#endif
