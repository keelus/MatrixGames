CC=g++
CFLAGS=
DIRECCION_EJECUTABLE=build
NOMBRE_EJECUTABLE=$(DIRECCION_EJECUTABLE)/hundirLaFlota

OBJETOS=mainFlota.o barco.o casilla.o tablero.o coordenada.o partida.o

all: $(NOMBRE_EJECUTABLE)

$(NOMBRE_EJECUTABLE): $(OBJETOS)
	mkdir -p $(DIRECCION_EJECUTABLE)
	$(CC) $(CFLAGS) -o $(NOMBRE_EJECUTABLE) $(OBJETOS)

mainFlota.o: src/mainFlota.cpp src/tablero.h src/partida.h
	$(CC) $(CFLAGS) -c src/mainFlota.cpp -o mainFlota.o

barco.o: src/barco.cpp src/barco.h src/casilla.h
	$(CC) $(CFLAGS) -c src/barco.cpp -o barco.o

casilla.o: src/casilla.cpp src/casilla.h
	$(CC) $(CFLAGS) -c src/casilla.cpp -o casilla.o

tablero.o: src/tablero.cpp src/tablero.h src/barco.h src/casilla.h
	$(CC) $(CFLAGS) -c src/tablero.cpp -o tablero.o

coordenada.o: src/coordenada.cpp src/coordenada.h
	$(CC) $(CFLAGS) -c src/coordenada.cpp -o coordenada.o

partida.o: src/partida.cpp src/partida.h src/coordenada.h
	$(CC) $(CFLAGS) -c src/partida.cpp -o partida.o

clean:
	rm -f *.o $(NOMBRE_EJECUTABLE)
