#ifndef SESION_H
#define SESION_H

#include "sql.h"
#include "tiposMenu.h"
#include <string>

class Sesion {
  public:
	Sesion();
	~Sesion();

	void SetMenuActual(TiposMenu menu);
	TiposMenu GetMenuActual();

	void SetEstadoLogin(EstadosMenuLogin estado);
	EstadosMenuLogin GetEstadoLogin();

	std::string GetNombreUsuario();
	int GetIdUsuario();

	std::string GetNombreIntroducidoLoginRegistro();
	void SetNombreIntroducidoLoginRegistro(std::string nombre);

	void SetSesion(int idUsuario, std::string nombreUsuario);
	void CerrarSesion();

  private:
	TiposMenu menuActual;
	EstadosMenuLogin estadoLogin;

	std::string nombreIntroducidoLoginRegistro;

	std::string nombreUsuario;
	int idUsuario;
};

#endif
