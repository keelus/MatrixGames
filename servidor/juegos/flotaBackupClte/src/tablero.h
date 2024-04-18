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

	Ataque RecibirAtaqueComoIA();
	bool AtaqueTacticoDisponibleIA();
	Coordenada CalcularAtaqueIA();

	Barco *BarcoEn(Coordenada coordenada);
	bool Colocable(TipoBarco tipo, Orientacion orientacion, int x, int y);
	bool AtaqueYaRecibido(Coordenada coordenada) const;
	Ataque RecibirAtaque(Coordenada coordenada);
	int BarcosRestantes() const;

  private:
	Barco Barcos[5];
	std::vector<Ataque> AtaquesRecibidos;
};

Tablero CrearTableroManualmente();
Tablero CrearTableroAleatoriamente();

#endif
