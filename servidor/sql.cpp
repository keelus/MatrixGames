#include <stdbool.h>
#include <iostream>
#include "externo/sqlite3pp/sqlite3pp.h"

bool credencialesCorrectas(const char *usuario, const char *contrasenya, int *idUsuario) {
	
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
bool crearUsuario(const char *usuario, const char *contrasenya, int *idusuario){

		sqlite3pp::database db("baseDeDatos.db");
			sqlite3pp::command cmd(
				db, "INSERT INTO usuario (nombre, contrasena) VALUES (:usuario, :contrasena)");
			cmd.bind(":usuario", usuario, sqlite3pp::nocopy);
			cmd.bind(":contrasena", contrasenya, sqlite3pp::nocopy);
			cmd.execute();		
	bool confirmar = credencialesCorrectas(usuario,contrasenya, idusuario);
	return confirmar;
		
}
bool verUsuario(const char *usuario ){
	sqlite3pp::database db("baseDeDatos.db");

	sqlite3pp::query qry(db, "SELECT id FROM USUARIO WHERE nombre = :nombre ");//Consulta
	qry.bind(":nombre", usuario, sqlite3pp::nocopy);//Une la busqueda
	int idUsuario = -1;//Torna el ID del usuario a -1
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {//For desde el principio al final del qry
		std::tie(idUsuario) = (*i).get_columns<int>(0);
	}

	return idUsuario >= 0;
}
bool verStats(const char *idUsuario){
	sqlite3pp::database db("baseDeDatos.db");//Esto conecta con la bd
	
	sqlite3pp::query qryA(db, "SELECT A.nombre, A.id, (MAX(puntuacion)) AS puntuacion_max FROM USUARIO A INNER JOIN PARTIDAS_SINGLEPLAYER B ON A.id = B.idUsuario GROUP BY B.idJuego WHERE A.id = :id ");
	qryA.bind(":id",idUsuario, sqlite3pp::nocopy);//Hacemos la consulta y le pasamos el Id del usuario que tenemos de referencia
	printf("\n");

	sqlite3pp::query qryBjUG(db, "SELECT (COUNT(B.RESULTADO)) AS partidas_jugadas, B.idJugador FROM USUARIO A INNER JOIN (SELECT BB.nombre, AA.resultado, AA.idJugador FROM PARTIDAS_MULTIPLAYER INNER JOIN TIPO_JUEGO ON AA.idJuego = BB.id) B ON A.id = B.idJugador GROUP BY B.idJuego WHERE A.id = :id ");
	qryBjUG.bind(":id",idUsuario, sqlite3pp::nocopy);//Carga la consulta de la cantidad de partidas jugadas por el jugador indicado agrupadas por cada juego
	
	sqlite3pp::query qryBgan(db, "SELECT (COUNT(B.RESULTADO)) AS partidas_ganadas,  B.idJugador FROM USUARIO A INNER JOIN (SELECT BB.nombre, AA.resultado, AA.idJugador FROM PARTIDAS_MULTIPLAYER INNER JOIN TIPO_JUEGO ON AA.idJuego = BB.id where AA.resultado = 1) B ON A.id = B.idJugador GROUP BY B.idJuego WHERE (A.id = :id) ");
	qryBgan.bind(":id",idUsuario, sqlite3pp::nocopy);//Carga las partidas ganadas por el usuario que se le pasa en las partidas multiplayer agrupadas por cada juego
	
	
	//std::tie(id, nombre) = (*i).get_columns<int, const char*>(0, 1);

    //std::cout << id << "\t" << (nombre ? nombre : "NULL") << std::endl;
	printf("PARTIDAS UN SOLO JUGADOR:\n");
	printf("juego    puntuacion max");
	for (sqlite3pp::query::iterator i = qryA.begin(); i != qryA.end(); ++i)
	{//Esto por lo que he conseguido entender deberia mostrar por pantalla lo que encuentra en las consultas pero ya se vera
		char* nombre;
		int id = 0;
		int puntuacionMax = 0;
		std::tie(nombre,id,puntuacionMax) = (*i).get_columns<char*, int, int>;
		
		std::cout << nombre << "   " << puntuacionMax << std::endl;
	}

	printf("PARTIDAS MULTIJUGADOR:\n");
	printf("Juego    partidas jugadas\n");
	for (sqlite3pp::query::iterator i = qryBjUG.begin(); i != qryBjUG.end(); ++i)
	{
		char* nombre;
		int id = 0;
		int cantPartJug = 0;
		std::tie(nombre,id,cantPartJug) = (*i).get_columns<char*, int, int>;

		std::cout << nombre << "    " << cantPartJug << std::endl;
	}

	printf("Juego    partidas ganadas\n");
	for (sqlite3pp::query::iterator i = qryBgan.begin(); i != qryBgan.end(); ++i)
	{
		char* nombre;
		int id = 0;
		int cantPartGan = 0;
		std::tie(nombre,id,cantPartGan) = (*i).get_columns<char*, int, int>;

		std::cout << nombre << "    " << cantPartGan << std::endl;
	}
	
	return true;
}