#ifndef BARCO_H
#define BARCO_H

#include "casilla.h"
#include <vector>

enum class TipoBarco {
	PORTAVIONES = 5,
	ACORAZADO = 4,
	CRUCERO = 3,
	SUBMARINO = 3,
	DESTRUCTOR = 2,
	NULO = 0,
};

enum class Orientacion {
	HORIZONTAL = 0,
	VERTICAL = 1,
};

class Barco {
  public:
	Barco(TipoBarco tipo, Orientacion orientacion, int x, int y, bool esGuia);
	Barco() : Tipo(TipoBarco::NULO), Orientacion(Orientacion::HORIZONTAL), X(0), Y(0), EsGuia(false) {
		for (int c = 0; c < int(Tipo); c++) { // Temporal. Necesario para testing
			Casillas.push_back(Casilla(Coordenada(0, 0), c, EstadoCasilla::BARCO));
		}
	};

	bool EstaHundido() const;
	bool EstaTocado() const;
	std::vector<Casilla> CasillasTocadas() const;
	std::vector<Coordenada> CoordenadasOcupadas() const;

	void Imprimir();

	TipoBarco Tipo;
	Orientacion Orientacion;
	int X;
	int Y;
	bool EsGuia;
	std::vector<Casilla> Casillas;
};

#endif
