#!/bin/bash

COMANDO_COMPILACION_SERVIDOR="g++ ./mainServidor.cpp ./utilsLED.cpp ./externo/rpi_ws281x/dma.o ./externo/rpi_ws281x/pwm.o ./externo/rpi_ws281x/ws2811.o ./externo/rpi_ws281x/mailbox.o ./externo/rpi_ws281x/pcm.o ./externo/rpi_ws281x/rpihw.o -lrt -lm ./sql.cpp ./menu.cpp ./paquete.cpp ./mensaje.cpp ./externo/sqlite3pp/sqlite3pp.cpp -lsqlite3 ./juegos/flota/barco.cpp ./juegos/flota/casilla.cpp ./juegos/flota/coordenada.cpp ./juegos/flota/partida.cpp ./juegos/flota/tablero.cpp -o mainServidor"

echo "## Compilando archivos rpi_ws281 a objetos ##"
for i in ./externo/rpi_ws281x/*.c
do
	echo "    gcc -c $i -o ${i%.c}.o"
	eval "gcc -c $i -o ${i%.c}.o"
done

echo "## Compilando servidor ##"
echo "    $COMANDO_COMPILACION_SERVIDOR"
eval "$COMANDO_COMPILACION_SERVIDOR"

echo "Fin de compilacion (revisa los mensajes anteriores, si hay errores)."
