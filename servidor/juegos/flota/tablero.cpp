#include "tablero.h"
#include "barco.h"
#include "casilla.h"
#include <algorithm>
#include <iostream>
#include <random>

void flota::Tablero::AContenidoColor(ColorLED referenciaContenido[8][8], bool esconderBarcos) const {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			referenciaContenido[i][j] = ColorCasilla(EstadoCasilla::AGUA);
		}
	}

	for (int i = 0; i < AtaquesRecibidos.size(); i++) {
		Ataque ataque = AtaquesRecibidos.at(i);
		if (ataque.EsHit) {
			referenciaContenido[ataque.Coord.Y][ataque.Coord.X] = ColorCasilla(EstadoCasilla::HIT);
		} else {
			referenciaContenido[ataque.Coord.Y][ataque.Coord.X] = ColorCasilla(EstadoCasilla::MISS);
		}
	}

	for (Barco barco : Barcos) {
		for (int c = 0; c < int(barco.Tipo); c++) {
			ColorLED color = ColorLED::Blanco;

			if (!esconderBarcos) {
				color = ColorCasilla(barco.Casillas[c].Estado);
			}

			if (barco.EsGuia) {
				color = ColorCasilla(EstadoCasilla::COLOCANDO);
			} else if (barco.EstaHundido()) {
				color = ColorCasilla(EstadoCasilla::HIT_HUNDIDO);
			}

			if (color != ColorLED::Blanco /* sin establecer */) {
				if (barco.Orientacion == Orientacion::HORIZONTAL) {
					referenciaContenido[barco.Y][barco.X + c] = color;
				} else {
					referenciaContenido[barco.Y + c][barco.X] = color;
				}
			}
		}
	}
}

std::string flota::Tablero::AString(bool esconderBarcos) const {
	std::string numeros[] = {u8"❶", u8"❷", u8"❸", u8"❹", u8"❺", u8"❻", u8"❼", u8"❽"};

	std::string outStr[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			outStr[i][j] = EmoticonoCasilla(EstadoCasilla::AGUA);
		}
	}

	for (int i = 0; i < AtaquesRecibidos.size(); i++) {
		Ataque ataque = AtaquesRecibidos.at(i);
		if (ataque.EsHit) {
			outStr[ataque.Coord.Y][ataque.Coord.X] = EmoticonoCasilla(EstadoCasilla::HIT);
		} else {
			outStr[ataque.Coord.Y][ataque.Coord.X] = EmoticonoCasilla(EstadoCasilla::MISS);
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

	std::string stringFinal = "";
	// SetConsoleOutputCP(CP_UTF8); // Necesario para emoticonos

	stringFinal.append("\n  🅰  🅱  🅲  🅳  🅴  🅵  🅶  🅷 \n");
	for (int i = 0; i < 8; ++i) {
		stringFinal.append(numeros[8 - i - 1]);
		stringFinal.append(" ");
		for (int j = 0; j < 8; ++j) {
			stringFinal.append(outStr[i][j]);
			stringFinal.append(" ");
		}
		stringFinal.append("\n");
	}

	return stringFinal;
}

void flota::Tablero::Imprimir(bool esconderBarcos) const {
	std::string numeros[] = {u8"❶", u8"❷", u8"❸", u8"❹", u8"❺", u8"❻", u8"❼", u8"❽"};

	std::string outStr[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			outStr[i][j] = EmoticonoCasilla(EstadoCasilla::AGUA);
		}
	}

	for (int i = 0; i < AtaquesRecibidos.size(); i++) {
		Ataque ataque = AtaquesRecibidos.at(i);
		if (ataque.EsHit) {
			outStr[ataque.Coord.Y][ataque.Coord.X] = EmoticonoCasilla(EstadoCasilla::HIT);
		} else {
			outStr[ataque.Coord.Y][ataque.Coord.X] = EmoticonoCasilla(EstadoCasilla::MISS);
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

	// SetConsoleOutputCP(CP_UTF8); // Necesario para emoticonos

	std::cout << "\n  🅰  🅱  🅲  🅳  🅴  🅵  🅶  🅷 \n";
	for (int i = 0; i < 8; ++i) {
		std::cout << numeros[8 - i - 1] << " ";
		for (int j = 0; j < 8; ++j) {
			std::cout << (outStr[i][j]) << " ";
		}
		std::cout << std::endl;
	}
};

flota::Tablero flota::CrearTableroAleatoriamente() {
	Tablero tablero;

	TipoBarco tiposBarco[5] = {TipoBarco::PORTAVIONES, TipoBarco::ACORAZADO, TipoBarco::CRUCERO, TipoBarco::SUBMARINO, TipoBarco::DESTRUCTOR};
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

void flota::Tablero::Colocar(Barco barco, int indice) { Barcos[indice] = barco; }

bool flota::Tablero::Colocable(TipoBarco tipo, Orientacion orientacion, int x, int y) {
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

flota::Barco *flota::Tablero::BarcoEn(Coordenada coordenada) {
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

flota::Tablero flota::CrearTableroManualmente() {
	Tablero tablero;

	TipoBarco tiposBarco[5] = {TipoBarco::PORTAVIONES, TipoBarco::ACORAZADO, TipoBarco::CRUCERO, TipoBarco::SUBMARINO, TipoBarco::DESTRUCTOR};

	for (int b = 0; b < 5; b++) {
		TipoBarco tipo = tiposBarco[b];

		// Creamos el barco que usaremos y lo colocamos
		Barco barco(tipo, Orientacion::VERTICAL, 0, 0, true);
		tablero.Colocar(barco, b);

		bool colocado = false;
		bool primerPrintHecho = false;
		while (!colocado) {
			if (!primerPrintHecho) {
				system("cls");
				tablero.Imprimir(false);
				primerPrintHecho = true;
			}

			/*
			if (kbhit()) {
				char teclaPulsada = getch();

				switch (teclaPulsada) {
				case 'A':
				case 'a':
					if (barco.X > 0) {
						barco.X--;
					}
					break;
				case 'D':
				case 'd':
					if (barco.Orientacion == Orientacion::HORIZONTAL && barco.X + int(barco.Tipo) < 8) {
						barco.X++;
					} else if (barco.Orientacion == Orientacion::VERTICAL && barco.X + 1 < 8) {
						barco.X++;
					}
					break;
				case 'W':
				case 'w':
					if (barco.Y > 0) {
						barco.Y--;
					}
					break;
				case 'S':
				case 's':
					if (barco.Orientacion == Orientacion::HORIZONTAL && barco.Y + 1 < 8) {
						barco.Y++;
					} else if (barco.Orientacion == Orientacion::VERTICAL && barco.Y + int(barco.Tipo) < 8) {
						barco.Y++;
					}
					break;
				case 'R':
				case 'r':
					if (barco.Orientacion == Orientacion::VERTICAL) {
						if (barco.X + int(barco.Tipo) >= 8) {
							barco.X += 8 - (barco.X + int(barco.Tipo));
						}
						barco.Orientacion = Orientacion::HORIZONTAL;
					} else {
						if (barco.Y + int(barco.Tipo) >= 8) {
							barco.Y += 8 - (barco.Y + int(barco.Tipo));
						}
						barco.Orientacion = Orientacion::VERTICAL;
					}
					break;
				case 'X':
				case 'x':
					exit(0);
					break;
				case 13:
					if (tablero.Colocable(barco.Tipo, barco.Orientacion, barco.X, barco.Y)) {
						barco.EsGuia = false;
						// Crear de nuevo el barco, para que las coordenadas de las casillas sean las correctas
						Barco barcoAColocar = Barco(barco.Tipo, barco.Orientacion, barco.X, barco.Y, false);
						tablero.Colocar(barcoAColocar, b);
						colocado = true;
					}
					break;
				}

				if (!colocado) {
					// Al haber pulsado una tecla que no sea enter, lo recolocamos
					tablero.Colocar(barco, b);
				}

				system("cls");
				tablero.Imprimir(false);
			}
			*/
		}
	}

	return tablero;
}

flota::Ataque flota::Tablero::RecibirAtaque(Coordenada coordenada) {
	Ataque ataque = Ataque(false, false, coordenada);

	Barco *barcoEncontrado = BarcoEn(coordenada);
	if (barcoEncontrado != nullptr) {
		int indice = (coordenada.X - barcoEncontrado->X) + (coordenada.Y - barcoEncontrado->Y);
		barcoEncontrado->Casillas.at(indice).Estado = EstadoCasilla::HIT;
		ataque.EsHundido = barcoEncontrado->EstaHundido();
		ataque.EsHit = true;
	}

	AtaquesRecibidos.push_back(ataque);

	return ataque;
}
int flota::Tablero::BarcosRestantes() const {
	int barcosRestantes = 0;
	for (int i = 0; i < 5; i++) {
		barcosRestantes += Barcos[i].EstaHundido() ? 0 : 1;
	}

	return barcosRestantes;
};

bool flota::Tablero::AtaqueYaRecibido(Coordenada coordenada) const {
	for (Ataque ataque : AtaquesRecibidos) {
		if (ataque.Coord.X == coordenada.X && ataque.Coord.Y == coordenada.Y) {
			return true;
		}
	}

	return false;
}

bool flota::Tablero::CompletamenteHundido() const {
	for (Barco barco : Barcos) {
		if (!barco.EstaHundido()) {
			return false;
		}
	}

	return true;
};

bool flota::Tablero::AtaqueTacticoDisponibleIA() {
	for (Barco barco : Barcos) {
		if (barco.EstaTocado() && !barco.EstaHundido()) {
			return true;
		}
	}

	return false;
}

flota::Coordenada flota::Tablero::CalcularAtaqueIA() {
	Coordenada coordenadasAtaque(-1, -1);

	// Si es posible, y la IA sabe que hay un barco tocado no hundido que puede atacar, intentara atacar alrededor
	if (AtaqueTacticoDisponibleIA()) {
		for (Barco barco : Barcos) {
			if (barco.EstaTocado() && !barco.EstaHundido()) {
				std::vector<Casilla> casillasTocadas = barco.CasillasTocadas();

				for (Casilla casillaTocada : casillasTocadas) {
					int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

					for (int i = 0; i < 4; i++) {
						int dir[2] = {direcciones[i][0], direcciones[i][1]};

						Coordenada coordenada(casillaTocada.Coord.X + dir[1], casillaTocada.Coord.Y + dir[0]);

						if (coordenada.Y >= 0 && coordenada.Y < 8 && coordenada.X >= 0 && coordenada.X < 8) {
							bool ataqueYaRecibido = AtaqueYaRecibido(coordenada);

							if (!ataqueYaRecibido) {
								coordenadasAtaque.X = coordenada.X;
								coordenadasAtaque.Y = coordenada.Y;

								return coordenadasAtaque;
							}
						}
					}
				}
			}
		}
	}

	// Si no es posible atacar tácticamente, elegir ataque aleatorio
	do {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribucionFilasYColumnas(0, 7); // Inclusivo

		coordenadasAtaque.X = distribucionFilasYColumnas(gen);
		coordenadasAtaque.Y = distribucionFilasYColumnas(gen);
	} while (AtaqueYaRecibido(coordenadasAtaque));

	return coordenadasAtaque;
}

flota::Ataque flota::Tablero::RecibirAtaqueComoIA() {
	Coordenada coordenadasAtaque = CalcularAtaqueIA();
	return RecibirAtaque(coordenadasAtaque);
}
