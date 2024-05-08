#ifndef PAQUETE_H
#define PAQUETE_H

#include "../cliente/input.h"
#include "sesion.h"
#include <iostream>

// Paquete => Mensaje que se manda desde el servidor hacia el cliente.
namespace paquetes {
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

void MandarPaquete(int socketId, Paquete paquete);
void MandarPaqueteDesconexion(int socketId);
void MandarPaquete(int socketId, std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla);
void MandarPaqueteDeMenu(int socketId, Sesion sesion, std::string mensajeError);
} // namespace paquetes

#endif
