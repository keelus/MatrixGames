#include "paquete.h"
#include <sys/socket.h>

void mandarPaquete(int socketId, Paquete paquete) {
	std::string paqueteStr = paquete.AString();
	send(socketId, paqueteStr.c_str(), paqueteStr.length(), 0);
	std::cout << "### Mensaje enviado a cliente ###" << std::endl;
}

void mandarPaquete(int socketId, std::string textoVisual, std::string preInput, ModosEntrada modoEntrada, bool limpiarPantalla) {
	Paquete paquete = {};
	paquete.TextoVisual = textoVisual;
	paquete.PreInput = preInput;

	std::string codigo = "2000";
	codigo[1] = modoEntrada == TEXTO ? '0' : '1';
	codigo[2] = limpiarPantalla ? '1' : '0';

	paquete.Codigo = codigo;

	mandarPaquete(socketId, paquete);
}
