#ifndef TABLERO_H
#define TABLERO_H

#include "../../colorLED.h"
#include "../../matrizLED.h"
#include "ataque.h"
#include "barco.h"
#include "coordenada.h"
#include "flota.h"
#include <vector>

namespace flota {
class Tablero {
  public:
	Tablero() : CoordenadaDeAtaque(0, 0) {}

	void Imprimir(bool esconderBarcos, bool mostrarCoordenadaDeAtaque) const;
	std::string AString(bool esconderBarcos, bool mostrarCoordenadaDeAtaque) const;
	void AContenidoColor(ColorLED referenciaContenido[8][8], bool esconderBarcos, bool mostrarCoordenadaDeAtaque) const;
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

	Coordenada CoordenadaDeAtaque; // Usada para visualizar a donde apunta el jugador, cuando sea necesario.

  private:
	Barco Barcos[5];
	std::vector<Ataque> AtaquesRecibidos;
};

flota::Tablero CrearTableroManualmente(int idSocket, MatrizLED *matrizLED);
flota::Tablero CrearTableroAleatoriamente();
} // namespace flota

#endif
