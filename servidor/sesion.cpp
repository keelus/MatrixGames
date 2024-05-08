#include "sesion.h"
#include "menu.h"
#include "sql.h"

Sesion::Sesion() {
	this->menuActual = MENU_0;
	this->estadoLogin = ESPERANDO_USUARIO;

	this->nombreIntroducidoLoginRegistro = "";

	this->nombreUsuario = "";
	this->idUsuario = -1;
}

Sesion::~Sesion() {}

void Sesion::SetMenuActual(TiposMenu menu) { this->menuActual = menu; }

TiposMenu Sesion::GetMenuActual() { return this->menuActual; }

void Sesion::SetEstadoLogin(EstadosMenuLogin estado) { this->estadoLogin = estado; }

EstadosMenuLogin Sesion::GetEstadoLogin() { return this->estadoLogin; }

std::string Sesion::GetNombreUsuario() { return this->nombreUsuario; }

int Sesion::GetIdUsuario() { return this->idUsuario; }

void Sesion::SetSesion(int idUsuario, std::string nombreUsuario) {
	this->nombreUsuario = nombreUsuario;
	this->idUsuario = idUsuario;

	this->nombreIntroducidoLoginRegistro = "";
	this->estadoLogin = ESPERANDO_USUARIO; // Reiniciar para futuros Logins
}

void Sesion::CerrarSesion() {
	this->menuActual = MENU_0;
	this->estadoLogin = ESPERANDO_USUARIO;

	this->nombreUsuario = "";
	this->nombreIntroducidoLoginRegistro = "";
	this->idUsuario = -1;
}

std::string Sesion::GetNombreIntroducidoLoginRegistro() { return this->nombreIntroducidoLoginRegistro; };
void Sesion::SetNombreIntroducidoLoginRegistro(std::string nombre) { this->nombreIntroducidoLoginRegistro = nombre; }
