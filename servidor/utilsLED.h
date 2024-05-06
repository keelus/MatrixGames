#ifndef UTILSLED_H
#define UTILSLED_H

#include "externo/rpi_ws281x/ws2811.h"
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

namespace utilsLED {
// clang-format off
enum class ColorLED { // Little endian!
	Rojo 		= 0x0000FF00,
	Verde 		= 0x000000FF,
	Azul 		= 0x00FF0000,

	Amarillo 	= 0x0000FFFF,
	Rosa 		= 0x00FF00FF,
	Cian 		= 0x00FFFF00,
	Naranja 	= 0x000030FF,

	Blanco 		= 0x00FFFFFF,
	Negro 		= 0x00000000
};

// clang-format on

class TiraLED {
  public:
	TiraLED();
	~TiraLED();

	void Limpiar();
	void Colorear(ColorLED color);
	void Renderizar();

	int anchura;
	int altura;
	int tamanyo;
	ws2811_t ledstring;
	ws2811_led_t *matriz;
};
} // namespace utilsLED

#endif
