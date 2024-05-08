#ifndef _MENSAJE_H_
#define _MENSAJE_H_

#include <iostream>

// Mensaje => Mensaje que se manda desde el cliente hacia el servidor.
namespace mensajes {
class Mensaje {
  public:
	Mensaje();
	std::string Contenido;
	bool SeQuiereDesconectar;
};
Mensaje LeerDesdeCliente(int socketId);
} // namespace mensajes

#endif
