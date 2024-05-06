#include "casilla.h"
#include <string>

std::string flota::EmoticonoCasilla(EstadoCasilla estado) {
	switch (estado) {
	case EstadoCasilla::AGUA:
		return u8"🟦";
	case EstadoCasilla::BARCO:
		return u8"🟫";
	case EstadoCasilla::COLOCANDO:
		return u8"🟩";
	case EstadoCasilla::HIT:
		return u8"🟨";
	case EstadoCasilla::HIT_HUNDIDO:
		return u8"🟧";
	case EstadoCasilla::MISS:
		return u8"🟥";
	}

	return "?";
}

ColorLED flota::ColorCasilla(EstadoCasilla estado) {
	switch (estado) {
	case EstadoCasilla::AGUA:
		return ColorLED::Cian;
	case EstadoCasilla::BARCO:
		return ColorLED::Blanco;
	case EstadoCasilla::COLOCANDO:
		return ColorLED::Verde;
	case EstadoCasilla::HIT:
		return ColorLED::Amarillo;
	case EstadoCasilla::HIT_HUNDIDO:
		return ColorLED::Negro;
	case EstadoCasilla::MISS:
		return ColorLED::Rojo;
	}

	return ColorLED::Rojo;
}
