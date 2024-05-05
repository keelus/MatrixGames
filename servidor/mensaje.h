#ifndef _MENSAJE_H_
#define _MENSAJE_H_

#include <iostream>

typedef struct MensajeDeCliente {
	bool desconectar;
	std::string contenido;
} MensajeDeCliente;

MensajeDeCliente leerDesdeCliente(int socketId);

#endif
