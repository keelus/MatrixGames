#include "partida.h"

namespace grave {

Partida::Partida() { this->personaje = Personaje(); };

void Partida::vaciarMapa() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			this->mapeado[i][j] = ColorLED::Negro;
		}
	}
}

void Partida::BuclePrincipal(MatrizLED *matriz, int socketUsuario) {
	// Primer mapa
	this->cargarMapa(1);
	matriz->SetMatrizColor(mapeado);

	while (true) {
		int mapaFinalizado = this->personaje.interaccion(&socketUsuario, &this->mapeado, matriz);

		if (mapaFinalizado)
			break;
	}

	// Segundo mapa
	this->cargarMapa(2);
	matriz->SetMatrizColor(mapeado);

	while (true) {
		int mapaFinalizado = this->personaje.interaccion(&socketUsuario, &this->mapeado, matriz);

		if (mapaFinalizado)
			break;
	}
}

void Partida::cargarMapa(int nivel) {
	this->vaciarMapa();
	switch (nivel) {
	case 1:
		for (int i = 0; i < 8; i++) {
			this->mapeado[0][i] = ColorLED::Naranja;
			this->mapeado[7][i] = ColorLED::Naranja;
			this->mapeado[i][7] = ColorLED::Naranja;
			this->mapeado[i][0] = ColorLED::Naranja;
		}

		this->mapeado[6][1] = ColorLED::Naranja;
		this->mapeado[6][2] = ColorLED::Naranja;
		this->mapeado[6][3] = ColorLED::Naranja;

		this->mapeado[5][1] = ColorLED::Naranja;

		this->mapeado[1][1] = ColorLED::Naranja;
		this->mapeado[2][1] = ColorLED::Naranja;

		this->mapeado[1][5] = ColorLED::Naranja;
		this->mapeado[2][5] = ColorLED::Naranja;

		this->mapeado[6][6] = ColorLED::Naranja;

		this->mapeado[4][1] = ColorLED::Amarillo;

		this->mapeado[5][2] = ColorLED::Verde;
		this->personaje.SetPosicion(2, 5);

		break;
	case 2:
		for (int i = 0; i < 8; i++) {
			this->mapeado[0][i] = ColorLED::Naranja;
			this->mapeado[7][i] = ColorLED::Naranja;
			this->mapeado[i][7] = ColorLED::Naranja;
			this->mapeado[i][0] = ColorLED::Naranja;
			this->mapeado[1][1] = ColorLED::Naranja;

			if (i <= 5)
				this->mapeado[4][i] = ColorLED::Naranja;

			if (i <= 3)
				this->mapeado[6][i] = ColorLED::Naranja;

			if (i <= 5 && i >= 2)
				this->mapeado[i][5] = ColorLED::Naranja;
		}

		this->mapeado[2][4] = ColorLED::Naranja;

		this->mapeado[2][3] = ColorLED::Amarillo;

		this->mapeado[5][1] = ColorLED::Verde;
		this->personaje.SetPosicion(1, 5);

		break;
	}
}

} // namespace grave
