#include <stdbool.h>

#include "externo/sqlite3pp/sqlite3pp.h"

bool credencialesCorrectas(const char *usuario, const char *contrasenya, const char *baseDeDatos, int *idUsuario) {
	sqlite3pp::database db("baseDeDatos.db");

	sqlite3pp::query qry(db, "SELECT id FROM USUARIO WHERE nombre = :nombre AND contrasena = :contrasena");
	qry.bind(":nombre", usuario, sqlite3pp::nocopy);
	qry.bind(":contrasena", usuario, sqlite3pp::nocopy);

	*idUsuario = -1;
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
		std::tie(*idUsuario) = (*i).get_columns<int>(0);
	}

	return *idUsuario >= 0;
}
