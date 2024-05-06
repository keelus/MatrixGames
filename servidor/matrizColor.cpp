#include "matrizColor.h"
#include "colorLED.h"
#include "juegos/flota/casilla.h"
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
	std::cout << "###### set contenido #######" << std::endl;
	for (unsigned int y = 0; y < this->Altura; y++) {
		for (unsigned int x = 0; x < this->Anchura; x++) {
			this->contenido[y][x] = nuevoContenido[y][x];
		}
	}
}

void MatrizColor::RellenarDeColor(ColorLED color) {
	std::cout << "###### rellenar de color #######" << std::endl;
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

void MatrizColor::Imprimir() const {
	std::string numeros[] = {u8"❶", u8"❷", u8"❸", u8"❹", u8"❺", u8"❻", u8"❼", u8"❽"};

	std::cout << "\n  🅰  🅱  🅲  🅳  🅴  🅵  🅶  🅷 \n";
	for (int i = 0; i < 8; ++i) {
		std::cout << numeros[8 - i - 1] << " ";
		for (int j = 0; j < 8; ++j) {
			std::string s = flota::Ftemporal(this->contenido[i][j]);
			std::cout << " " << s << " ";
		}
		std::cout << std::endl;
	}
};
