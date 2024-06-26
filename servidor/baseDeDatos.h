#ifndef _SQLITE_
#define _SQLITE_

#include "sesion.h"
#include <stdbool.h>
#include <string>

namespace baseDeDatos {
bool CredencialesCorrectas(std::string usuario, std::string contrasenya, int *idUsuario);
bool CrearUsuario(std::string usuario, std::string contrasenya, int *idUsuario);
bool VerUsuario(std::string usuario);
std::string ConseguirEstadisticas(Sesion sesion);
void GrabarPartidaUnJugador(Sesion sesion, int idJuego, int puntuacion);
void GrabarPartidaMultijugador(Sesion sesion, int idJuego, int duracionSegundos, int resultado);
} // namespace baseDeDatos

#endif
