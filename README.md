# MatrixGames

### Identificacion
El usuario default para iniciar sesion es "admin" y la contraseña "admin".

### Requerimentos
Para poder compilar el servidor, necesitarás tener instalado `sqlite3` y `libc++` instalado correctamente.

### Compilar
#### Comando para el servidor
```
cd ./servidor/
```
Y luego (en linux)
```
g++ ./mainServidor.cpp ./sql.cpp ./menu.cpp ./paquete.cpp ./mensaje.cpp ./externo/sqlite3pp/sqlite3pp.cpp -lsqlite3 ./juegos/flota/barco.cpp ./juegos/flota/casilla.cpp ./juegos/flota/coordenada.cpp ./juegos/flota/partida.cpp ./juegos/flota/tablero.cpp -o mainServidor
```

#### Comando para el cliente
```
cd ./cliente/
```
Y luego
```
gcc mainCliente.c input.c mensajes.c -lws2_32 -o mainCliente.exe
```

#### Temporal (comando para compilar archivo temporal de pruebas ws2812)
```
gcc main.c dma.c pwm.c ws2811.c mailbox.c pcm.c rpihw.c -lrt -lm -o main
```
