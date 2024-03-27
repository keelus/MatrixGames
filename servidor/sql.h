#ifndef _SQLITE_
#define _SQLITE_

#include <stdbool.h>

enum EstadosMenuLogin { ESPERANDO_USUARIO, ESPERANDO_CONTRASENYA };
typedef enum EstadosMenuLogin EstadosMenuLogin;

bool credencialesCorrectas(const char *usuario, const char *contrasenya, const char *baseDeDatos, int *idUsuario);

#endif
