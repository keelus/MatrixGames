#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#include "../../colorLED.h"
#include "../../matrizLED.h"

namespace grave {
class Personaje {
  private:
	int x, y;

  public:
	Personaje();
	void SetPosicion(int x, int y);

	int interaccion(int *socketUsuario, ColorLED (*mapa)[8][8], MatrizLED *matriz);
	int movimiento(char tecla, ColorLED (*mapa)[8][8], MatrizLED *matriz);
};

} // namespace grave

#endif
