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
g++ ./mainServidor.cpp ./externo/rpi_ws281x/dma.o ./externo/rpi_ws281x/pwm.o ./externo/rpi_ws281x/ws2811.o ./externo/rpi_ws281x/mailbox.o ./externo/rpi_ws281x/pcm.o ./externo/rpi_ws281x/rpihw.o -lrt -lm ./sql.cpp ./menu.cpp ./paquete.cpp ./mensaje.cpp ./externo/sqlite3pp/sqlite3pp.cpp -lsqlite3 ./juegos/flota/barco.cpp ./juegos/flota/casilla.cpp ./juegos/flota/coordenada.cpp ./juegos/flota/partida.cpp ./juegos/flota/tablero.cpp ./utilsLED.cpp -o mainServidor
```
##### En caso de ser necesario, para compilar la libreria ws2811
Al ser C, primero la compilamos con gcc para despues usarla desde el servidor. 
Estando dentro de la carpeta `servidor/externo/rpi_ws281x/`:
```
gcc -c ws2811.c -o ws2811.o
gcc -c dma.c -o dma.o
gcc -c pwm.c -o pwm.o
gcc -c mailbox.c -o mailbox.o
gcc -c pcm.c -o pcm.o
gcc -c rpihw.c -o rpihw.o

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


