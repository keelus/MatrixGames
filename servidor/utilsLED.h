#ifndef UTILSLED_H
#define UTILSLED_H

#include "externo/rpi_ws281x/ws2811.h"
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

namespace utilsLED {
// clang-format off
enum class ColorLED {
	Rojo,
	Verde,
	Azul,
	Blanco,
	Negro
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
