#ifndef TABLERO_H
#define TABLERO_H

#include "ataque.h"
#include "barco.h"
#include <vector>

class Tablero {
  public:
	Tablero() {}

	void Imprimir(bool esconderBarcos) const;
	void Colocar(Barco barco, int indice);
	bool CompletamenteHundido() const;
	Ataque AtacarComoIA();
	Barco *BarcoEn(Coordenada coordenada);
	bool Colocable(TipoBarco tipo, Orientacion orientacion, int x, int y);
	bool AtaqueYaRealizado(Coordenada coordenada) const;
	Ataque RecibirAtaque(Coordenada coordenada);
	int BarcosRestantes() const;

	Barco ConseguirBarco(int i) const;
	std::vector<Ataque> ConseguirAtaques() const;

  private:
	Barco Barcos[5];
	std::vector<Ataque> Ataques;
};

Tablero CrearTableroManualmente();
Tablero CrearTableroAleatoriamente();

#endif
