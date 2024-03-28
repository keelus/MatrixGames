#include <stdbool.h>

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

	sqlite3pp::query qry(db, "SELECT id FROM USUARIO WHERE nombre = :nombre ");
	qry.bind(":nombre", usuario, sqlite3pp::nocopy);
	int idUsuario = -1;
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
		std::tie(idUsuario) = (*i).get_columns<int>(0);
	}

	return idUsuario >= 0;
}