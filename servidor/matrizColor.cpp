#include "matrizColor.h"
#include "colorLED.h"
#include <iostream>

MatrizColor::MatrizColor() { Limpiar(); }

MatrizColor::~MatrizColor() {}

void MatrizColor::ColorearPixel(unsigned int x, unsigned int y, ColorLED color) {
	if (x < 8 && y < 8) {
		this->contenido[y][x] = color;
	} else {
		std::cout << "Error al poner el pixel de color. Coordenadas fuera de rango (x: " << x << ", y: " << y << ")" << std::endl;
		exit(1);
	}
}

void MatrizColor::SetContenido(ColorLED nuevoContenido[8][8]) {
	for (unsigned int y = 0; y < this->Altura; y++) {
		for (unsigned int x = 0; x < this->Anchura; x++) {
			this->contenido[y][x] = nuevoContenido[y][x];
		}
	}
}

void MatrizColor::RellenarDeColor(ColorLED color) {
	for (unsigned int y = 0; y < this->Altura; y++) {
		for (unsigned int x = 0; x < this->Anchura; x++) {
			this->contenido[y][x] = color;
		}
	}
}

ColorLED MatrizColor::getPixel(unsigned int x, unsigned int y) {
	if (x < 8 && y < 8) {
		return this->contenido[y][x];
	} else {
		std::cout << "Error al conseguir el pixel. Coordenadas fuera de rango (x: " << x << ", y: " << y << ")" << std::endl;
		exit(1);
	}
}

void MatrizColor::Limpiar() { RellenarDeColor(ColorLED::Negro); }
