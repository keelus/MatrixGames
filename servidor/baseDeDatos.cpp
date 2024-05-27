#include "baseDeDatos.h"
#include "externo/sqlite3pp/sqlite3pp.h"
#include "fecha.h"
#include "sesion.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdbool.h>
#include <string>

#define ID_JUEGO_SNAKE 1
#define ID_JUEGO_FLAPPY 2
#define ID_JUEGO_SLIPGRAVE 3
#define ID_JUEGO_FLOTA 4
#define ID_JUEGO_4RAYA 5

bool baseDeDatos::CredencialesCorrectas(std::string usuario, std::string contrasenya, int *idUsuario) {

	sqlite3pp::database db("baseDeDatos.db");

	sqlite3pp::query qry(db, "SELECT id FROM USUARIO WHERE nombre = :nombre AND contrasena = :contrasena");
	qry.bind(":nombre", usuario, sqlite3pp::nocopy);
	qry.bind(":contrasena", contrasenya, sqlite3pp::nocopy);

	*idUsuario = -1;
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
		std::tie(*idUsuario) = (*i).get_columns<int>(0);
	}

	return *idUsuario >= 0;
}

bool baseDeDatos::CrearUsuario(std::string usuario, std::string contrasenya, int *idusuario) {

	sqlite3pp::database db("baseDeDatos.db");
	sqlite3pp::command cmd(db, "INSERT INTO usuario (nombre, contrasena) VALUES (:usuario, :contrasena)");
	cmd.bind(":usuario", usuario, sqlite3pp::nocopy);
	cmd.bind(":contrasena", contrasenya, sqlite3pp::nocopy);
	cmd.execute();
	bool confirmar = CredencialesCorrectas(usuario, contrasenya, idusuario);
	return confirmar;
}

bool baseDeDatos::VerUsuario(std::string usuario) {
	sqlite3pp::database db("baseDeDatos.db");

	sqlite3pp::query qry(db, "SELECT id FROM USUARIO WHERE nombre = :nombre "); // Consulta
	qry.bind(":nombre", usuario, sqlite3pp::nocopy);							// Une la busqueda
	int idUsuario = -1;															// Torna el ID del usuario a -1
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {		// For desde el principio al final del qry
		std::tie(idUsuario) = (*i).get_columns<int>(0);
	}

	return idUsuario >= 0;
}

// TODO: Testear esto
std::string baseDeDatos::ConseguirEstadisticas(Sesion sesion) {
	sqlite3pp::database db("baseDeDatos.db"); // Esto conecta con la bd

	// Ya que solo hay 2 juegos programados (un singleplayer y uno multi), a mano:
	sqlite3pp::query querySlipGrave(db, "SELECT puntuacion FROM PARTIDAS_SINGLEPLAYER WHERE idUsuario=:idUsuario and idJuego=:idJuego");
	querySlipGrave.bind(":idUsuario", sesion.GetIdUsuario());
	querySlipGrave.bind(":idJuego", ID_JUEGO_SLIPGRAVE);

	sqlite3pp::query queryFlota(db, "SELECT resultado FROM PARTIDAS_MULTIPLAYER WHERE idUsuario=:idUsuario and idJuego=:idJuego");
	queryFlota.bind(":idUsuario", sesion.GetIdUsuario());
	queryFlota.bind(":idJuego", ID_JUEGO_FLOTA);

	int cantidadPartidasSlipGrave = 0;
	int puntuacionMaximaSlipGrave = 0;

	int cantidadPartidasFlota = 0;
	int cantidadVictoriasFlota = 0;

	for (sqlite3pp::query::iterator i = querySlipGrave.begin(); i != querySlipGrave.end(); ++i) {
		cantidadPartidasSlipGrave++;
		int puntuacion = 0;
		std::tie(puntuacion) = (*i).get_columns<int>(0);

		if (puntuacion > puntuacionMaximaSlipGrave)
			puntuacionMaximaSlipGrave = puntuacion;
	}

	for (sqlite3pp::query::iterator i = queryFlota.begin(); i != queryFlota.end(); ++i) {
		cantidadPartidasFlota++;
		int resultado = 0;
		std::tie(resultado) = (*i).get_columns<int>(0);

		if (resultado == 1)
			cantidadVictoriasFlota++;
	}

	std::stringstream textoRespuesta;
	textoRespuesta << "Tus estadisticas" << std::endl << std::endl;
	textoRespuesta << "JUEGO\t\t\tRECORD\tVICTORIAS\tPARTIDAS JUGADAS" << std::endl;
	textoRespuesta << "Snake\t\t\t-\t-\t\t-" << std::endl;
	textoRespuesta << "Flappy Bird\t\t-\t-\t\t-" << std::endl;
	textoRespuesta << "Slip Grave\t\t" << puntuacionMaximaSlipGrave << "\t-\t\t" << cantidadPartidasSlipGrave << std::endl;
	textoRespuesta << "Hundir la flota\t\t-\t" << cantidadVictoriasFlota << "\t\t" << cantidadPartidasFlota << std::endl;
	textoRespuesta << "4 en raya\t\t-\t-\t\t-" << std::endl;

	return textoRespuesta.str();
}

void baseDeDatos::GrabarPartidaMultijugador(Sesion sesion, int idJuego, int duracionSegundos, int resultado) {
	sqlite3pp::database db("baseDeDatos.db");
	sqlite3pp::command cmd(db, "INSERT INTO PARTIDAS_MULTIPLAYER (idUsuario, fechaUnix, idJuego, duracionS, resultado) VALUES (:idUsuario, :fechaUnix, :idJuego, :duracionS, :resultado)");

	auto ahora = std::chrono::system_clock::now();
	std::time_t ahora_t = std::chrono::system_clock::to_time_t(ahora);

	int ahoraUnix = static_cast<int>(ahora_t);

	cmd.bind(":idUsuario", sesion.GetIdUsuario());
	cmd.bind(":fechaUnix", ahoraUnix);
	cmd.bind(":idJuego", idJuego);
	cmd.bind(":duracionS", duracionSegundos);
	cmd.bind(":resultado", resultado);
	cmd.execute();
}

void baseDeDatos::GrabarPartidaUnJugador(Sesion sesion, int idJuego, int puntuacion) {
	sqlite3pp::database db("baseDeDatos.db");
	sqlite3pp::command cmd(db, "INSERT INTO PARTIDAS_SINGLEPLAYER (idUsuario, fechaUnix, idJuego, puntuacion) VALUES (:idUsuario, :fechaUnix, :idJuego, :puntuacion)");

	auto ahora = std::chrono::system_clock::now();
	std::time_t ahora_t = std::chrono::system_clock::to_time_t(ahora);

	int ahoraUnix = static_cast<int>(ahora_t);

	cmd.bind(":idUsuario", sesion.GetIdUsuario());
	cmd.bind(":fechaUnix", ahoraUnix);
	cmd.bind(":idJuego", idJuego);
	cmd.bind(":puntuacion", puntuacion);
	cmd.execute();
}
