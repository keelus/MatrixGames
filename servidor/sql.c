#include "sql.h"
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool credencialesCorrectas(const char *usuario, const char *contrasenya, const char *baseDeDatos, int *idUsuario) {
	sqlite3 *db;

	int rc = sqlite3_open(baseDeDatos, &db);
	if (rc != SQLITE_OK) {
		printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	} else {
		printf("Base de datos abierta correctamente\n");
	}

	// Realizar operaciones con la base de datos...
	const char *sql = "SELECT id FROM usuario WHERE nombre = ? AND contrasena = ?;";
	sqlite3_stmt *stmt;
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_text(stmt, 1, usuario, strlen(usuario), SQLITE_STATIC);
	rc = sqlite3_bind_text(stmt, 2, contrasenya, strlen(contrasenya), SQLITE_STATIC);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		*idUsuario = sqlite3_column_int(stmt, 0);
	}
	printf("ID: %i\n", *idUsuario);

	sqlite3_finalize(stmt);
	// Cerrar la base de datos cuando hayas terminado
	sqlite3_close(db);

	return *idUsuario >= 0;
}
