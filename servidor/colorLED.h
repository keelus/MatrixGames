#ifndef COLORLED_H
#define COLORLED_H

#include <iostream>

// clang-format off
enum class ColorLED { // Formato hex: 0xRRGGBB
	Rojo 		= 0xFF0000,
	Verde 		= 0x00FF00,
	Azul 		= 0x0000FF,

	Amarillo 	= 0xFFFF00,
	Rosa 		= 0xFF00FF,
	Cian 		= 0x00FFFF,
	Naranja 	= 0xFF3000,

	Blanco 		= 0xFFFFFF,
	Negro 		= 0x000000,
};

std::string ColorString(ColorLED color);

// clang-format on

#endif
