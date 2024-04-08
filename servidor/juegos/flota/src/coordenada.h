#ifndef COORDENADA_H
#define COORDENADA_H

class Coordenada {
  public:
	int X, Y;

	Coordenada(int x, int y) : X(x), Y(y){};

	bool operator==(const Coordenada &otra) const { return X == otra.X && Y == otra.Y; }
};

#endif
