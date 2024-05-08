#include "mensajes.h"
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

#define TAMANO_BUFFER 1024

mensajes::Mensaje::Mensaje() {
	this->Contenido = "";
	this->SeQuiereDesconectar = false;
}

mensajes::Mensaje mensajes::LeerDesdeCliente(int socketId) {
	ssize_t valread;
	char buffer[TAMANO_BUFFER] = {0};

	Mensaje mensaje;

	while (true) {
		valread = read(socketId, buffer, TAMANO_BUFFER - 1);

		if (valread == 0) { // Desconexion
			mensaje.SeQuiereDesconectar = true;
			return mensaje;
		}

		if (buffer[0] != '\0' && buffer[0] != '\\') { // Es valido
			std::cout << "> Recibido desde cliente: \"" << buffer << "\" [tamano: " << valread << ", " << strlen(buffer) << "]" << std::endl;
			mensaje.SeQuiereDesconectar = false;
			mensaje.Contenido = buffer;

			return mensaje;
		} else {
			std::cout << "\t> Ignorado: \"" << buffer << "\" [tamano: " << valread << ", " << strlen(buffer) << "]" << std::endl;
		}

		// TODO: Contar intentos, y establecer un limite, para evitar bucles infinitos
	}
}
