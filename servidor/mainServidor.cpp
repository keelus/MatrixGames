#include "logger.h"
#include "matrizLED.h"
#include "paquetes.h"
#include "sesion.h"
#include "tiposMenu.h"
#include <ctime>
#include <string>
#undef UNICODE

#include "baseDeDatos.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Flota
#include "juegos/flota/partida.h"
#include "juegos/slipGrave/partida.h"
#define TAMANO_BUFFER 1024
#define DEFAULT_PORT 3000

#define USANDO_RASPBERRY_CON_MATRIZ_LED true
MatrizLED *matrizLED;

#define ID_JUEGO_SNAKE 1
#define ID_JUEGO_FLAPPY 2
#define ID_JUEGO_SLIPGRAVE 3
#define ID_JUEGO_FLOTA 4
#define ID_JUEGO_4RAYA 5

Logger logger;
Sesion sesion;
int socketUsuario;

void BuclePrincipal();

int main(void) {
	logger.Log("Servidor iniciado.", CategoriaLog::Otro);

	matrizLED = new MatrizLED(USANDO_RASPBERRY_CON_MATRIZ_LED);
	matrizLED->RellenarDeColor(ColorLED::Blanco);

	logger.Log("Matriz LED iniciada.", CategoriaLog::Otro);

	printf("__ __  __ _____ ___ ___   __ \n"
		   "|  V  |/  \\_   _| _ \\ \\ \\_/ / \n"
		   "| \\_/ | /\\ || | | v / |> , <  \n"
		   "|_|_|_|_||_||_| |_|_\\_/_/ \\_\\ \n"
		   " / _]/  \\|  V  | __/' _/      \n"
		   "| [/\\ /\\ | \\_/ | _|`._`.      \n"
		   " \\__/_||_|_| |_|___|___/\n"
		   "  ====  SERVIDOR  ====  \n");

	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	};

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(DEFAULT_PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	logger.Log("Esperando conexion.", CategoriaLog::Otro);
	std::cout << "Esperando conexion." << std::endl;

	// Escuchar y esperar a la conexion de un usuario
	if ((socketUsuario = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	// Bucle principal con la logica del servidor, menus, juegos, etc.
	// Una vez se salga de este, significara que el usuario se ha desconectado, o se ha perdido la conexion.
	logger.Log("Usuario conectado.", CategoriaLog::Conexion);
	std::cout << "Usuario conectado." << std::endl;

	BuclePrincipal();

	matrizLED->RellenarDeColor(ColorLED::Negro);

	if (sesion.GetNombreUsuario() == "") {
		logger.Log("Usuario desconectado.", CategoriaLog::Desconexion);
	} else {
		logger.Log("Usuario desconectado. (\" " + sesion.GetNombreUsuario() + " \").", CategoriaLog::Desconexion);
	}

	std::cout << "Un cliente se ha desconectado. Cerrando servidor." << std::endl;

	close(socketUsuario);
	close(server_fd); // Cerrar escucha
	return 0;
}

void BuclePrincipal() {
	std::string ultimoError = "";
	paquetes::MandarPaqueteDeMenu(socketUsuario, sesion, ultimoError);

	while (true) {
		matrizLED->RellenarDeColor(ColorLED::Naranja);

		paquetes::PaqueteDeCliente paqueteDeCliente = paquetes::LeerPaqueteDesdeCliente(socketUsuario);
		if (paqueteDeCliente.SeQuiereDesconectar())
			break;

		switch (sesion.GetMenuActual()) {
		case TiposMenu::Menu0: {
			char accionElegida = paqueteDeCliente.GetContenido()[0];

			if (accionElegida == '1') {
				sesion.SetMenuActual(TiposMenu::Menu0_Login);
			} else if (accionElegida == '2') {
				sesion.SetMenuActual(TiposMenu::Menu0_Registro);
			} else if (accionElegida == '3') {
				paquetes::MandarPaqueteDesconexion(socketUsuario);
				return;
			}

			break;
		}
		case TiposMenu::Menu1: {
			char accionElegida = paqueteDeCliente.GetContenido()[0];

			if (accionElegida == '1') {
				sesion.SetMenuActual(TiposMenu::Menu2); // Ir al menu de juegos
			} else if (accionElegida == '2') {
				sesion.SetMenuActual(TiposMenu::Menu3); // Ir a estadisticas
			} else if (accionElegida == '3') {
				paquetes::MandarPaqueteDesconexion(socketUsuario);
				return;
			}
			break;
		}
		case TiposMenu::Menu2: {
			char accionElegida = paqueteDeCliente.GetContenido()[0];

			// estas opciones son para iniciar los juesgos, menos el 6 que es para ir al menu anterior
			if (accionElegida == '1') {
				ultimoError = "Este juego no esta implementado...\n";
				// BUCLE PRINCIPAL SNAKE

			} else if (accionElegida == '2') {
				ultimoError = "Este juego no esta implementado...\n";
				// BUCLE PRINCIPAL FLAPPY BIRD

			} else if (accionElegida == '3') {
				// BUCLE PRINCIPAL SLIP GRAVE
				logger.Log("Iniciando juego \"grave\".", CategoriaLog::Partida);
				std::cout << "Se desea jugar a slip grave" << std::endl;

				grave::Partida partida;
				partida.BuclePrincipal(matrizLED, socketUsuario);

				baseDeDatos::GrabarPartidaUnJugador(sesion, ID_JUEGO_SLIPGRAVE, 1000);

			} else if (accionElegida == '4') { // Hundir la flota (vs CPU)
				std::time_t inicio = std::time(nullptr);

				logger.Log("Iniciando juego \"flota\".", CategoriaLog::Partida);
				std::cout << "Se desea jugar a hundir la flota" << std::endl;
				flota::Partida partida(socketUsuario, matrizLED);

				while (!partida.HaFinalizado()) {
					bool desconectar = partida.Iteracion(socketUsuario, matrizLED);

					if (desconectar) {
						paquetes::MandarPaqueteDesconexion(socketUsuario);
						return;
					}
				}

				bool resultado = partida.TableroJugador.CompletamenteHundido() ? 0 : 1;
				int duracionPartida = static_cast<int>(difftime(std::time(nullptr), inicio));

				baseDeDatos::GrabarPartidaMultijugador(sesion, ID_JUEGO_FLOTA, duracionPartida, resultado);

				if (partida.TableroJugador.CompletamenteHundido()) {
					std::cout << "Has perdido! No te quedan mas barcos. Suerte a la proxima!";
					paquetes::MandarPaquete(socketUsuario, "Has perdido! No te quedan mas barcos. Suerte a la proxima!", "\n[ Pulsa una tecla para finalizar el juego ]", PULSACION, true);

					matrizLED->RellenarDeColor(ColorLED::Rojo);
				} else {
					std::cout << "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!";
					paquetes::MandarPaquete(socketUsuario, "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!", "\n[ Pulsa una tecla para finalizar el juego ]", PULSACION, true);

					matrizLED->RellenarDeColor(ColorLED::Verde);
				}

				paquetes::LeerPaqueteDesdeCliente(socketUsuario); // Bloquear hasta respuesta
				logger.Log("Finalizado juego \"flota\".", CategoriaLog::Partida);

				break;
			} else if (accionElegida == '5') {
				ultimoError = "Este juego no esta implementado...\n";
				// BUCLE PRINCIPAL 4 EN RAYA

			} else if (accionElegida == '6') {
				sesion.SetMenuActual(TiposMenu::Menu1);
				// Devuelve a la pestaña anterior
			}
			break;
		}
		case TiposMenu::Menu0_Login: {
			std::string textoIntroducido = paqueteDeCliente.GetContenido();

			switch (sesion.GetEstadoLogin()) {
			case EstadosLoginRegistro::EsperandoUsuario: {
				sesion.SetNombreIntroducidoLoginRegistro(textoIntroducido);
				sesion.SetEstadoLogin(EstadosLoginRegistro::EsperandoContrasenya);
				break;
			}
			case EstadosLoginRegistro::EsperandoContrasenya: {
				int idUsuario = -1;
				bool correctas = baseDeDatos::CredencialesCorrectas(sesion.GetNombreIntroducidoLoginRegistro(), textoIntroducido, &idUsuario);
				sesion.SetSesion(idUsuario, sesion.GetNombreIntroducidoLoginRegistro());

				if (correctas) {
					// Si es correcta, iniciamos sesion correctamente
					logger.Log("Usuario logueado (\"" + sesion.GetNombreUsuario() + "\").", CategoriaLog::Otro);
					ultimoError = "";
					sesion.SetMenuActual(TiposMenu::Menu1);
				} else {
					// Si no lo es, limpiamos usuario, y pasamos al menu 0.
					std::string mensajeError = "El usuario y/o contrasena son incorrectos. Intentalo de nuevo o crea una cuenta.";

					ultimoError = mensajeError;

					sesion.SetMenuActual(TiposMenu::Menu0);
					sesion.SetNombreIntroducidoLoginRegistro("");
				}
				break;
			}
			}
			break;
		}
		case TiposMenu::Menu0_Registro: {
			std::string textoIntroducido = paqueteDeCliente.GetContenido();

			switch (sesion.GetEstadoLogin()) {
			case EstadosLoginRegistro::EsperandoUsuario: {
				sesion.SetNombreIntroducidoLoginRegistro(textoIntroducido);

				bool existe = baseDeDatos::VerUsuario(sesion.GetNombreIntroducidoLoginRegistro());
				if (existe) {
					std::string mensajeError = "Este usuario ya existe, por favor cree la cuenta con otro nombre de usuario.";
					ultimoError = mensajeError;

					sesion.SetMenuActual(TiposMenu::Menu0);
					sesion.SetNombreIntroducidoLoginRegistro("");
					break;
				}
				sesion.SetEstadoLogin(EstadosLoginRegistro::EsperandoContrasenya);
				break;
			}
			case EstadosLoginRegistro::EsperandoContrasenya: {
				int idUsuario = -1;

				bool correctas = baseDeDatos::CrearUsuario(sesion.GetNombreIntroducidoLoginRegistro(), textoIntroducido, &idUsuario);
				if (correctas) {
					ultimoError = "";
					sesion.SetSesion(idUsuario, sesion.GetNombreIntroducidoLoginRegistro());

					logger.Log("Usuario registrado (\" " + sesion.GetNombreUsuario() + " \").", CategoriaLog::Otro);

					sesion.SetMenuActual(TiposMenu::Menu1);
				} else {
					std::string mensajeError = "Este usuario ya existe, por favor cree la cuenta con otro nombre de usuario.";
					ultimoError = mensajeError;

					sesion.SetMenuActual(TiposMenu::Menu0);
					sesion.SetNombreIntroducidoLoginRegistro("");
					sesion.SetEstadoLogin(EstadosLoginRegistro::EsperandoUsuario);
				}
				break;
			}
			}
			break;
		}
		case TiposMenu::Menu3: { // Menu de estadisticas
			// El menu ya se habra mostrado (en el paquete al principio del switch).
			// Cuando lleguemos aqui, significa que el usuario ha pulsado una tecla para volver atras.
			sesion.SetMenuActual(TiposMenu::Menu1);
			break;
		}
		}

		paquetes::MandarPaqueteDeMenu(socketUsuario, sesion, ultimoError);
	}
}
