#ifndef _SQLITE_
#define _SQLITE_

#include <stdbool.h>

enum EstadosLogin { SIN_INICIAR, ESPERANDO_PASSWORD, INICIADO };
typedef enum EstadosLogin EstadosLogin;

bool credencialesCorrectas(const char *usuario, const char *contrasenya, const char *baseDeDatos, int *idUsuario);

#endif
