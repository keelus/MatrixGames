#ifndef _SQLITE_
#define _SQLITE_

#include <stdbool.h>
#include <string>

enum EstadosMenuLogin { ESPERANDO_USUARIO, ESPERANDO_CONTRASENYA };
typedef enum EstadosMenuLogin EstadosMenuLogin;

bool CredencialesCorrectas(std::string usuario, std::string contrasenya, int *idUsuario);
bool CrearUsuario(std::string usuario, std::string contrasenya, int *idUsuario);
bool VerUsuario(std::string usuario);
#endif
