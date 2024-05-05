#include "barco.h"
#include "casilla.h"

flota::Barco::Barco(TipoBarco tipo, enum Orientacion orientacion, int x, int y, bool esGuia) {
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

		Casilla nuevaCasilla(Coordenada(columnaCasilla, filaCasilla), c, EstadoCasilla::BARCO);
		this->Casillas.push_back(nuevaCasilla);
	}
}

bool flota::Barco::EstaHundido() const {
	for (Casilla casilla : Casillas) {
		if (casilla.Estado == EstadoCasilla::BARCO) {
			return false;
		}
	}

	return true;
}

bool flota::Barco::EstaTocado() const {
	for (Casilla casilla : Casillas) {
		if (casilla.Estado == EstadoCasilla::HIT) {
			return true;
		}
	}

	return false;
}

std::vector<flota::Coordenada> flota::Barco::CoordenadasOcupadas() const {
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

std::vector<flota::Casilla> flota::Barco::CasillasTocadas() const {
	std::vector<Casilla> casillasTocadas;

	for (Casilla casilla : Casillas) {
		if (casilla.Estado == EstadoCasilla::HIT) {
			casillasTocadas.push_back(casilla);
		}
	}

	return casillasTocadas;
};

void flota::Barco::Imprimir() {
	std::cout << "\nBarco{ \n\tOrientacion: " << (Orientacion == Orientacion::HORIZONTAL ? "horizontal" : "vertical") << ", \n\tX: " << X << ", \n\tY:" << Y << ",\n\tCasillas: {";

	for (Casilla casilla : Casillas) {
		std::cout << "\n\t\t{Indice: " << casilla.Indice << ",X: " << casilla.Coord.X << ", Y: " << casilla.Coord.Y << "}";
	}

	std::cout << "\n\t}\n}";
}
