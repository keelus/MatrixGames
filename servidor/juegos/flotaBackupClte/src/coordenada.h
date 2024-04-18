#ifndef COORDENADA_H
#define COORDENADA_H

#include <iostream>

class Coordenada {
  public:
	int X, Y;

	Coordenada(int x, int y) : X(x), Y(y){};

	bool operator==(const Coordenada &otra) const { return X == otra.X && Y == otra.Y; }
};

Coordenada ParsearCoordenada(std::string coordenadaStr);

#endif
