#ifndef MATRIZCOLOR_H
#define MATRIZCOLOR_H

#include "colorLED.h"

// Matriz que usara cada juego, para almacenar su estado actual en colores
class MatrizColor {
  public:
	MatrizColor();
	~MatrizColor();

	void ColorearPixel(unsigned int x, unsigned int y, ColorLED color);
	void SetContenido(ColorLED contenido[8][8]);
	void RellenarDeColor(ColorLED color);
	void Limpiar();

	ColorLED getPixel(unsigned int x, unsigned int y);

	const unsigned int Anchura = 8;
	const unsigned int Altura = 8;

  private:
	ColorLED contenido[8][8];
};

#endif
