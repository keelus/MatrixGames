#ifndef _SQLITE_
#define _SQLITE_

#include "sesion.h"
#include <stdbool.h>
#include <string>

bool CredencialesCorrectas(std::string usuario, std::string contrasenya, int *idUsuario);
bool CrearUsuario(std::string usuario, std::string contrasenya, int *idUsuario);
bool VerUsuario(std::string usuario);
void GrabarPartidaMultijugador(Sesion sesion, int idJuego, int duracionSegundos, int resultado);

#endif
