#include "paquete.h"
#include <sys/socket.h>

void mandarPaquete(int socketId, Paquete paquete) {
	std::string paqueteStr = paquete.AString();
	send(socketId, paqueteStr.c_str(), paqueteStr.length(), 0);
	std::cout << "### Mensaje enviado a cliente ###" << std::endl;
}
