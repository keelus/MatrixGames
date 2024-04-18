#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>

class Paquete {
      public:
	Paquete() : TextoVisual(""), PreInput(""), Codigo(""){};
	Paquete(std::string textoVisual, std::string preInput, std::string codigo) : TextoVisual(textoVisual), PreInput(preInput), Codigo(codigo){};

	std::string AString() {
		std::string stringFinal = Codigo.append(";").append(TextoVisual).append(";").append(PreInput).append(";");

		return stringFinal;
	}

	std::string TextoVisual;
	std::string PreInput;
	std::string Codigo;
};

#endif
