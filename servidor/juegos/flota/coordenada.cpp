#include "coordenada.h"
#include <map>
#include <stdexcept>

Coordenada ParsearCoordenada(std::string coordenadaStr) {
	std::map<char, int> letrasValidas = {
		{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7},
	};

	std::map<char, int> numerosValidos = {
		{'1', 7}, {'2', 6}, {'3', 5}, {'4', 4}, {'5', 3}, {'6', 2}, {'7', 1}, {'8', 0},
	};

	if (coordenadaStr.length() == 2) {
		int letra = coordenadaStr[0];
		int numero = coordenadaStr[1];

		int columna = -1;
		int fila = -1;
		try {
			columna = letrasValidas.at(letra);
			fila = numerosValidos.at(numero);

			return Coordenada(columna, fila);
		} catch (std::out_of_range &e) {
			throw "Coordenada invalida";
		}
	} else {
		throw "Coordenada invalida";
	}
}
