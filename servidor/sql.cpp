#include "sql.h"
#include "externo/sqlite3pp/sqlite3pp.h"
#include "fecha.h"
#include "sesion.h"
#include <iostream>
#include <stdbool.h>

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
bool baseDeDatos::VerStats(std::string idUsuario) {
	sqlite3pp::database db("baseDeDatos.db"); // Esto conecta con la bd

	sqlite3pp::query qryA(db, "SELECT A.nombre, A.id, (MAX(puntuacion)) AS puntuacion_max FROM USUARIO A INNER JOIN PARTIDAS_SINGLEPLAYER B ON A.id = B.idUsuario GROUP BY B.idJuego WHERE A.id = :id ");
	qryA.bind(":id", idUsuario, sqlite3pp::nocopy); // Hacemos la consulta y le pasamos el Id del usuario que tenemos de referencia
	printf("\n");

	sqlite3pp::query qryBjUG(db, "SELECT (COUNT(B.RESULTADO)) AS partidas_jugadas, B.idJugador FROM USUARIO A INNER JOIN (SELECT BB.nombre, AA.resultado, AA.idJugador FROM PARTIDAS_MULTIPLAYER INNER JOIN TIPO_JUEGO ON AA.idJuego = BB.id) B ON A.id = B.idJugador GROUP BY B.idJuego WHERE A.id = :id ");
	qryBjUG.bind(":id", idUsuario, sqlite3pp::nocopy); // Carga la consulta de la cantidad de partidas jugadas por el jugador indicado agrupadas por cada juego

	sqlite3pp::query qryBgan(db, "SELECT (COUNT(B.RESULTADO)) AS partidas_ganadas,  B.idJugador FROM USUARIO A INNER JOIN (SELECT BB.nombre, AA.resultado, AA.idJugador FROM PARTIDAS_MULTIPLAYER INNER JOIN TIPO_JUEGO ON AA.idJuego = BB.id where AA.resultado = 1) B ON A.id = B.idJugador GROUP BY B.idJuego WHERE (A.id = :id) ");
	qryBgan.bind(":id", idUsuario, sqlite3pp::nocopy); // Carga las partidas ganadas por el usuario que se le pasa en las partidas multiplayer agrupadas por cada juego

	// Muestra la puntuacion maxima de cada juego conseguida por el jugador
	printf("PARTIDAS UN SOLO JUGADOR:\n");
	printf("juego    puntuacion max");
	for (sqlite3pp::query::iterator i = qryA.begin(); i != qryA.end(); ++i) {
		char const *nombre;
		int id = 0;
		int puntuacionMax = 0;
		std::tie(nombre, id, puntuacionMax) = (*i).get_columns<char const *, int, int>(0, 1, 2);

		std::cout << nombre << "   " << puntuacionMax << std::endl;
	}

	// Muestra la cantidad de partidas jugadas por caga juego multijugador (contra PC)
	printf("PARTIDAS MULTIJUGADOR:\n");
	printf("Juego    partidas jugadas\n");
	for (sqlite3pp::query::iterator i = qryBjUG.begin(); i != qryBjUG.end(); ++i) {
		char const *nombre;
		int id = 0;
		int cantPartJug = 0;
		std::tie(nombre, id, cantPartJug) = (*i).get_columns<char const *, int, int>(0, 1, 2);

		std::cout << nombre << "    " << cantPartJug << std::endl;
	}

	// Muestra la cantidad de partidas ganadas por caga juego multijugador (contra PC)
	// Estos querys estan actualizados? todos son iguales y aqui aparecen como que reciben 3 resultados, pero veo las SQL no todas reciben esto
	printf("Juego    partidas ganadas\n");
	for (sqlite3pp::query::iterator i = qryBgan.begin(); i != qryBgan.end(); ++i) {
		char const *nombre;
		int id = 0;
		int cantPartGan = 0;
		std::tie(nombre, id, cantPartGan) = (*i).get_columns<char const *, int, int>(0, 1, 2);

		std::cout << nombre << "    " << cantPartGan << std::endl;
	}

	return true;
}

void GrabarPartidaMultijugador(Sesion sesion, int idJuego, int duracionSegundos, int resultado) {
	sqlite3pp::database db("baseDeDatos.db");
	sqlite3pp::command cmd(db, "INSERT INTO PARTIDAS_MULTIPLAYER (idJugador, fecha, idJuego, duracionS, resultado) VALUES (:idJugador, :fecha, :idJuego, :duracionS, :resultado)");

	int dia, mes, anyo, hora, minuto, segundo;
	Fecha::ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

	cmd.bind(":idJugador", sesion.GetIdUsuario());
	cmd.bind(":fecha", Fecha::ConseguirFechaVisual(), sqlite3pp::nocopy);
	cmd.bind(":idJuego", idJuego);
	cmd.bind(":duracionS", duracionSegundos);
	cmd.bind(":resultado", resultado);
	cmd.execute();
}
