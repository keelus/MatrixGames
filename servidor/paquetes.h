#ifndef PAQUETES_H
#define PAQUETES_H

#include <iostream>

#include "../cliente/input.h"
#include "sesion.h"

namespace paquetes {
class PaqueteDeServidor {
  public:
	PaqueteDeServidor(std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla);
	PaqueteDeServidor(std::string textoVisual, std::string preInput, std::string codigo);

	std::string AString();

  private:
	std::string textoVisual;
	std::string preInput;
	std::string codigo;
};

void MandarPaquete(int socketId, PaqueteDeServidor paquete);
void MandarPaqueteDesconexion(int socketId);
void MandarPaquete(int socketId, std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla);
void MandarPaqueteDeMenu(int socketId, Sesion sesion, std::string mensajeError);

class PaqueteDeCliente {
  public:
	PaqueteDeCliente(std::string contenido, bool quiereDesconectarse);

	std::string GetContenido();
	bool SeQuiereDesconectar();

  private:
	std::string contenido;
	bool desconectar;
};

PaqueteDeCliente LeerPaqueteDesdeCliente(int socketId);
} // namespace paquetes

#endif
