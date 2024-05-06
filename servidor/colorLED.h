#ifndef COLORLED_H
#define COLORLED_H

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
	Negro 		= 0x00000000,

	Nulo 		= 0x00000000 // Para realizar comprobaciones. No borrar.
};

// clang-format on

#endif
