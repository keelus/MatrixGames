#include "tablero.h"
#include "barco.h"
#include "casilla.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <windows.h>

void Tablero::Imprimir(bool esconderBarcos) const {
	std::string numeros[] = {u8"❶", u8"❷", u8"❸", u8"❹", u8"❺", u8"❻", u8"❼", u8"❽"};

	std::string outStr[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			outStr[i][j] = EmoticonoCasilla(EstadoCasilla::AGUA);
		}
	}

	for (Barco barco : Barcos) {
		for (int c = 0; c < int(barco.Tipo); c++) {
			std::string simbolo = " ";

			if (!esconderBarcos) {
				simbolo = EmoticonoCasilla(barco.Casillas[c].Estado);
			}

			if (barco.EsGuia) {
				simbolo = EmoticonoCasilla(EstadoCasilla::COLOCANDO);
			} else if (barco.EstaHundido()) {
				simbolo = EmoticonoCasilla(EstadoCasilla::HIT_HUNDIDO);
			}

			if (simbolo != " ") {
				if (barco.Orientacion == Orientacion::HORIZONTAL) {
					outStr[barco.Y][barco.X + c] = simbolo;
				} else {
					outStr[barco.Y + c][barco.X] = simbolo;
				}
			}
		}
	}

	SetConsoleOutputCP(CP_UTF8); // Necesario para emoticonos

	std::cout << "\n  🅰  🅱  🅲  🅳  🅴  🅵  🅶  🅷 \n";
	for (int i = 0; i < 8; ++i) {
		std::cout << numeros[8 - i - 1] << " ";
		for (int j = 0; j < 8; ++j) {
			std::cout << (outStr[i][j]) << " ";
		}
		std::cout << std::endl;
	}
};

Tablero CrearTableroAleatoriamente() {
	Tablero tablero;

	TipoBarco tiposBarco[5] = {TipoBarco::PORTAVIONES, TipoBarco::CRUCERO, TipoBarco::ACORAZADO, TipoBarco::SUBMARINO, TipoBarco::DESTRUCTOR};
	Orientacion orientaciones[2] = {Orientacion::HORIZONTAL, Orientacion::VERTICAL};

	// Inicializar generador de numberos aleatorios STD
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribucionOrientacion(0, 1); // Inclusivo

	for (int b = 0; b < 5; b++) {
		TipoBarco tipo = tiposBarco[b];
		Orientacion orientacion = orientaciones[distribucionOrientacion(gen)];

		int filaMax = 8;
		int columnaMax = 8;

		if (orientacion == Orientacion::HORIZONTAL) {
			columnaMax -= (int(tipo) - 1);
		} else {
			filaMax -= (int(tipo) - 1);
		}

		bool colocado = false;
		std::uniform_int_distribution<> distribucionFilas(0, filaMax - 1);		 // Inclusivo
		std::uniform_int_distribution<> distribucionColumnas(0, columnaMax - 1); // Inclusivo
		while (!colocado) {
			int fila = distribucionFilas(gen);
			int columna = distribucionColumnas(gen);

			bool colocable = tablero.Colocable(tipo, orientacion, columna, fila);

			if (colocable) {
				Barco barco(tipo, orientacion, columna, fila, false);
				tablero.Colocar(barco, b);
				colocado = true;
			}
		}
	}

	return tablero;
}

void Tablero::Colocar(Barco barco, int indice) { Barcos[indice] = barco; }

bool Tablero::Colocable(TipoBarco tipo, Orientacion orientacion, int x, int y) {
	for (int c = 0; c < int(tipo); c++) {
		Coordenada coordenadaABuscar(x, y);
		if (orientacion == Orientacion::HORIZONTAL) {
			coordenadaABuscar.X += c;
		} else {
			coordenadaABuscar.Y += c;
		}

		Barco *barcoEncontrado = BarcoEn(coordenadaABuscar);
		if (barcoEncontrado != nullptr) {
			return false;
		}
	}

	return true;
}

Barco *Tablero::BarcoEn(Coordenada coordenada) {
	for (int b = 0; b < 5; b++) {
		Barco *barco = &Barcos[b];
		std::vector<Coordenada> coordenadasOcupadas = barco->CoordenadasOcupadas();

		if (std::find(coordenadasOcupadas.begin(), coordenadasOcupadas.end(), coordenada) != coordenadasOcupadas.end()) {
			// Si contiene
			if (!barco->EsGuia) {
				return barco;
			}
		}
	}

	return nullptr;
}
