# MatrixGames

### Identificacion
El usuario default para iniciar sesion es "admin" y la contraseña "admin".

### Requerimentos
Para poder compilar el servidor, necesitarás tener instalado `sqlite3` y `libc++` instalado correctamente.

Para que las leds funcionen, ejecutar como administrador (sudo) el programa `./mainServidor`, en linux.

### Compilar
#### Comando para el servidor (en Linux)
##### Compilar.sh
Puedes ejecutar el archivo `./compilar.sh` desde la carpeta `servidor/`. Si no, mira el siguiente apartado
##### Manualmente
Primero, compilamos la libreria `rpi_ws2811` en archivos `.o` con `gcc`, para luego poder linkearlos y usarlos con `g++`.
Estando dentro de la carpeta `servidor/externo/rpi_ws281x/` ejecutamos:
```
gcc -c ws2811.c -o ws2811.o
gcc -c dma.c -o dma.o
gcc -c pwm.c -o pwm.o
gcc -c mailbox.c -o mailbox.o
gcc -c pcm.c -o pcm.o
gcc -c rpihw.c -o rpihw.o
```
Despues, estando en la carpeta raiz del servidor (`servidor/`) ejecutamos:
```
g++ ./mainServidor.cpp ./matrizLED.cpp ./colorLED.cpp ./externo/rpi_ws281x/dma.o ./externo/rpi_ws281x/pwm.o ./externo/rpi_ws281x/ws2811.o ./externo/rpi_ws281x/mailbox.o ./externo/rpi_ws281x/pcm.o ./externo/rpi_ws281x/rpihw.o -lrt -lm ./baseDeDatos.cpp ./paquetes.cpp ./logger.cpp ./sesion.cpp ./fecha.cpp ./externo/sqlite3pp/sqlite3pp.cpp -lsqlite3 ./juegos/flota/barco.cpp ./juegos/flota/casilla.cpp ./juegos/flota/coordenada.cpp ./juegos/flota/partida.cpp ./juegos/flota/tablero.cpp ./juegos/deslizar/mapa.cpp ./juegos/deslizar/personaje.cpp -o mainServidor
```

#### Comando para el cliente (en Windows)
```
cd ./cliente/
```
Y luego
```
gcc mainCliente.c input.c mensajes.c -lws2_32 -o mainCliente.exe
```
