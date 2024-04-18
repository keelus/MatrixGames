#ifndef _SQLITE_
#define _SQLITE_

#include <stdbool.h>
#include <string>

enum EstadosMenuLogin { ESPERANDO_USUARIO, ESPERANDO_CONTRASENYA };
typedef enum EstadosMenuLogin EstadosMenuLogin;

bool credencialesCorrectas(std::string usuario, std::string contrasenya, int *idUsuario);
bool crearUsuario(std::string usuario, std::string contrasenya, int *idUsuario);
bool verUsuario(std::string usuario);
#endif
