#include "casilla.h"
#include <string>

ColorLED flota::ColorCasilla(EstadoCasilla estado) {
	switch (estado) {
	case EstadoCasilla::AGUA:
		return ColorLED::Azul;
	case EstadoCasilla::BARCO:
		return ColorLED::Blanco;
	case EstadoCasilla::COLOCANDO:
		return ColorLED::Verde;
	case EstadoCasilla::ATACANDO:
		return ColorLED::Naranja;
	case EstadoCasilla::HIT:
		return ColorLED::Amarillo;
	case EstadoCasilla::HIT_HUNDIDO:
		return ColorLED::Negro;
	case EstadoCasilla::MISS:
		return ColorLED::Rojo;
	}

	return ColorLED::Rojo;
}
