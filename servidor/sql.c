#include "sql.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h> 

int *verificarUsuario(char *user, const char *database) {
    sqlite3 *db;
    int *rest = (int *)malloc(sizeof(int));
    if (rest == NULL) {
        printf("Error: no se pudo asignar memoria.\n");
        exit(EXIT_FAILURE);
    }
    *rest = -1;

    char *err_msg = 0;
    int rc = sqlite3_open(database, &db);
    if (rc != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rest;
    } else {
        printf("Base de datos abierta correctamente\n");
    }
    
    // Realizar operaciones con la base de datos...
    const char *sql = "SELECT id FROM usuario WHERE nombre = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc = sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error en la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rest;
    }

    // Ejecutar la consulta y procesar los resultados
    int id = -1; // Inicializa id como un entero normal, no un puntero
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }
    printf("ID: %i\n", id);
    
    sqlite3_finalize(stmt);
    // Cerrar la base de datos cuando hayas terminado
    sqlite3_close(db);
    *rest = id; // Asigna el valor de id al puntero rest
    return rest;
}

void *verificarContrasenya(int *id, char *contrasenya){
    
}

