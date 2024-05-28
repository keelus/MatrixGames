#ifndef PARTIDA_H_
#define PARTIDA_H_

#include "../../colorLED.h"
#include "../../matrizLED.h"
#include "personaje.h"

namespace grave {
class Partida {
  public:
	Partida();
	void BuclePrincipal(MatrizLED *matriz, int socketUsuario);

  private:
	ColorLED mapeado[8][8];
	Personaje personaje;

	void vaciarMapa();
	void cargarMapa(int nivel);
};

} // namespace grave

#endif
