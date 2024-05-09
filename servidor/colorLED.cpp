#include "colorLED.h"

std::string ColorString(ColorLED color) {
	switch (color) {
	case ColorLED::Cian:
		return "Cian";
	case ColorLED::Blanco:
		return "Blanco";
	case ColorLED::Verde:
		return "Verde";
	case ColorLED::Amarillo:
		return "Amarillo";
	case ColorLED::Negro:
		return "Negro";
	case ColorLED::Rojo:
		return "Rojo";
	case ColorLED::Azul:
		return "Azul";
	case ColorLED::Naranja:
		return "Naranja";
	case ColorLED::Rosa:
		return "Rosa";
	}

	return "ColorDesconocido";
}
