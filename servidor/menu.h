#ifndef _MENUH_
#define _MENUH_

#include "paquete.h"
#include "sesion.h"
#include "sql.h"
#include "tiposMenu.h"
#include <string>

Paquete CrearPaqueteDeMenu(Sesion sesion, std::string mensajeError);

#endif
