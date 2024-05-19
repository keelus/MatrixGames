#ifndef PERSONAJE_H_
#define PERSONAJE_H_
#include "../../colorLED.h"
#include "../../matrizLED.h"

namespace grave {
class Personaje {
  public:
	Personaje();
	int interaccion(int *socketUsuario, ColorLED (*mapa)[8][8], MatrizLED *matriz);
	int movimiento(char tecla, ColorLED (*mapa)[8][8],MatrizLED* matriz);
	~Personaje();
	int x;
	int y;
};

} // namespace grave

#endif
