#include "partida.h"
#include "tablero.h"

int main() {
	Tablero tablero1;

	// tablero1 = CrearTableroAleatoriamente();

	Partida partida;

	while (!partida.HaFinalizado()) {
		partida.Iteracion();
	}
}
