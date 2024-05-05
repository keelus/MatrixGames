#ifndef ATAQUE_H
#define ATAQUE_H

#include "coordenada.h"
#include "flota.h"

namespace flota {
class Ataque {
      public:
	Ataque(bool esHit, bool esHundido, flota::Coordenada coord) : EsHit(esHit), EsHundido(esHundido), Coord(coord){};
	bool EsHit, EsHundido;
	flota::Coordenada Coord;
};
} // namespace flota

#endif
