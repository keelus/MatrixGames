#ifndef PARTIDA_H
#define PARTIDA_H

#include "flota.h"
#include "tablero.h"

namespace flota {
enum class TiposTurno {
	TURNO_JUGADOR = 0,
	TURNO_CPU = 1,
};

class Partida {
      public:
	Partida() {
		// TableroJugador = CrearTableroManualmente();
		TableroJugador = CrearTableroAleatoriamente();
		TableroCPU = CrearTableroAleatoriamente();
		Turno = TiposTurno::TURNO_JUGADOR;
	};
	bool Iteracion(int socketId);
	bool HaFinalizado();
	Tablero TableroJugador;
	Tablero TableroCPU;
	TiposTurno Turno;
};

} // namespace flota

#endif
