#ifndef UTILSLED_H
#define UTILSLED_H

#include "colorLED.h"
#include "externo/rpi_ws281x/ws2811.h"
#include <iostream>
#include <stdlib.h>
#include <unordered_map>

class MatrizLED {
  public:
	MatrizLED(bool usandoRaspberry);
	~MatrizLED();

	const unsigned int anchura = 8;
	const unsigned int altura = 8;
	const unsigned int tamanyo = anchura * altura;

	ColorLED MatrizColor[8][8];
	void SetPixel(unsigned int x, unsigned int y, ColorLED colorLED);
	ColorLED GetPixel(unsigned int x, unsigned int y);
	void SetMatrizColor(ColorLED nuevoContenido[8][8]);
	void RellenarDeColor(ColorLED colorLED);
	void Limpiar();

	void ImprimirMatrizColor();

	bool UsandoRaspberry;

	ws2811_t ledstring;
	ws2811_led_t *matriz;

  private:
	void dibujar();
	void renderizar();
};

#endif
