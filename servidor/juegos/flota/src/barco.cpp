#include "barco.h"
#include "casilla.h"

Barco::Barco(TipoBarco tipo, enum Orientacion orientacion, int x, int y, bool esGuia) {
	this->Tipo = tipo;
	this->Orientacion = orientacion;
	this->X = x;
	this->Y = y;
	this->EsGuia = esGuia;

	for (int c = 0; c < int(tipo); c++) {
		int filaCasilla = y;
		int columnaCasilla = x;

		if (orientacion == Orientacion::HORIZONTAL) {
			columnaCasilla += c;
		} else {
			filaCasilla += c;
		}

		Casilla nuevaCasilla(Coordenada(filaCasilla, columnaCasilla), c, EstadoCasilla::BARCO);
		this->Casillas.push_back(nuevaCasilla);
	}
}

bool Barco::EstaHundido() const { return false; }

std::vector<Coordenada> Barco::CoordenadasOcupadas() const {
	std::vector<Coordenada> coordenadas;

	for (int c = 0; c < int(Tipo); c++) {
		Coordenada coordenadaOcupada(X, Y);

		if (Orientacion == Orientacion::HORIZONTAL) {
			coordenadaOcupada.X += c;
		} else {
			coordenadaOcupada.Y += c;
		}

		coordenadas.push_back(coordenadaOcupada);
	}

	return coordenadas;
}
