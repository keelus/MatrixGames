#include "partida.h"
#include "tablero.h"

int main() {
	Partida partida;

	while (!partida.HaFinalizado()) {
		partida.Iteracion();
	}

	if (partida.TableroJugador.CompletamenteHundido()) {
		std::cout << "Has perdido! No te quedan mas barcos. Suerte a la proxima!";
	} else {
		std::cout << "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!";
	}
}

void
