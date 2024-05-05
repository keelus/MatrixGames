# MatrixGames

### Identificacion
El usuario default para iniciar sesion es "admin" y la contraseña "admin".

### Requerimentos
Para poder compilar el servidor, necesitarás tener instalado `sqlite3` y `libc++` instalado correctamente.

Para que las leds funcionen, ejecutar como administrador (sudo) el programa `./mainServidor`, en linux.

### Compilar
#### Comando para el servidor (en Linux)
Primero, compilamos la libreria `rpi_ws2811` en archivos `.o` con `gcc`, para luego poder linkearlos y usarlos con `g++`.
Estando dentro de la carpeta `servidor/externo/rpi_ws281x/`:
```
gcc -c ws2811.c -o ws2811.o
gcc -c dma.c -o dma.o
gcc -c pwm.c -o pwm.o
gcc -c mailbox.c -o mailbox.o
gcc -c pcm.c -o pcm.o
gcc -c rpihw.c -o rpihw.o

```
Despues, estando en la carpeta `servidor/`
```
g++ ./mainServidor.cpp ./externo/rpi_ws281x/dma.o ./externo/rpi_ws281x/pwm.o ./externo/rpi_ws281x/ws2811.o ./externo/rpi_ws281x/mailbox.o ./externo/rpi_ws281x/pcm.o ./externo/rpi_ws281x/rpihw.o -lrt -lm ./sql.cpp ./menu.cpp ./paquete.cpp ./mensaje.cpp ./externo/sqlite3pp/sqlite3pp.cpp -lsqlite3 ./juegos/flota/barco.cpp ./juegos/flota/casilla.cpp ./juegos/flota/coordenada.cpp ./juegos/flota/partida.cpp ./juegos/flota/tablero.cpp ./utilsLED.cpp -o mainServidor
```

#### Comando para el cliente (en Windows)
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


