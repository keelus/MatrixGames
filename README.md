# MatrixGames

### Requerimentos
Para poder compilar el servidor, necesitarás tener instalado `sqlite3` y `libc++` instalado correctamente.

### Compilar
#### Comando para el servidor
```
cd ./servidor/
```
Y luego
```
g++ .\mainServidor.cpp .\sql.cpp .\menu.cpp .\externo\sqlite3pp\sqlite3pp.cpp -lws2_32 -lsqlite3 .\juegos\flota\barco.cpp .\juegos\flota\casilla.cpp .\juegos\flota\coordenada.cpp .\juegos\flota\partida.cpp .\juegos\flota\tablero.cpp -o mainServidor.exe
```

#### Comando para el cliente
```
cd ./cliente/
```
Y luego
```
gcc mainCliente.c input.c mensajes.c -lws2_32 -o mainCliente.exe
```
