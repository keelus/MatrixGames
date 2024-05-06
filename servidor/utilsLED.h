#ifndef UTILSLED_H
#define UTILSLED_H

#include "externo/rpi_ws281x/ws2811.h"
#include "matrizColor.h"
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

namespace utilsLED {

class TiraLED {
  public:
	TiraLED();
	~TiraLED();

	void Colorear(MatrizColor matrizColor);

	int anchura;
	int altura;
	int tamanyo;

	ws2811_t ledstring;
	ws2811_led_t *matriz;

  private:
	void renderizar();
};
} // namespace utilsLED

#endif
