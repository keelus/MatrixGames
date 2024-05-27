#ifndef MAPA_H_
#define MAPA_H_
#include "../../colorLED.h"
#include "../../matrizLED.h"
#include "personaje.h"
namespace grave {
class Mapa {
  public:
	Mapa(MatrizLED *matriz, int socketUsuario);
	void cargarmapas(int nivel);
	void mapacolor(ColorLED mapeado[8][8], MatrizLED *matriz);
	~Mapa();

  private:
	ColorLED mapeado[8][8];
	Personaje personaje;
};

} // namespace grave

#endif
