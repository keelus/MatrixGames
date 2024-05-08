#include "logger.h"
#include "matrizLED.h"
#include "mensaje.h"
#include "menu.h"
#include "paquete.h"
#include "sesion.h"
#include "tiposMenu.h"
#include <string>
#undef UNICODE

#include "menu.h"
#include "sql.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// Flota
#include "juegos/flota/partida.h"

#define TAMANO_BUFFER 1024
#define DEFAULT_PORT 3000

#define USANDO_RASPBERRY_CON_MATRIZ_LED false
MatrizLED *matrizLED;

Logger logger;
Sesion sesion;

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

	int server_fd, new_socket;
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
		logger.Log("Esperando conexion.", CategoriaLog::Otro);
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	logger.Log("Usuario conectado.", CategoriaLog::Conexion);

	std::string ultimoError = "";

	Paquete paquete = CrearPaqueteDeMenu(sesion, ultimoError);
	std::string paqueteStr = paquete.AString();

	mandarPaquete(new_socket, paquete);

	while (true) {
		matrizLED->RellenarDeColor(ColorLED::Naranja);

		MensajeDeCliente mensajeDeCliente = leerDesdeCliente(new_socket);
		if (mensajeDeCliente.desconectar)
			break;

		switch (sesion.GetMenuActual()) {
		case TiposMenu::Menu0: {
			char accionElegida = mensajeDeCliente.contenido[0];

			if (accionElegida == '1') {
				sesion.SetMenuActual(TiposMenu::Menu0_Login);
			} else if (accionElegida == '2') {
				sesion.SetMenuActual(TiposMenu::Menu0_Registro);
			} else if (accionElegida == '3') {
				// Desconectar usuario
				sesion.SetMenuActual(TiposMenu::Cerrar);
			} else {
				// Error!
			}

			break;
		}
		case TiposMenu::Menu1: {
			char accionElegida = mensajeDeCliente.contenido[0];
			if (accionElegida == '1') {
				sesion.SetMenuActual(TiposMenu::Menu2);
			} else if (accionElegida == '2') {
				sesion.SetMenuActual(TiposMenu::Menu3);
			} else if (accionElegida == '3') {

			} else if (accionElegida == '4') {
				// Desconectar usuario
				sesion.SetMenuActual(TiposMenu::Cerrar);
			} else {
				// Error!
			}
			break;
		}
		case TiposMenu::Menu2: {
			char accionElegida = mensajeDeCliente.contenido[0];
			// estas opciones son para iniciar los juesgos, menos el 6 que es para ir al menu anterior
			if (accionElegida == '1') { // Snake

			} else if (accionElegida == '2') { // Flappy Bird

			} else if (accionElegida == '3') { // Slip Grave

			} else if (accionElegida == '4') { // Hundir la flota (vs CPU)
				logger.Log("Iniciando juego \"flota\".", CategoriaLog::Partida);
				std::cout << "Se desea jugar a hundir la flota" << std::endl;
				flota::Partida partida;

				// Prueba: Mandar tablero al jugador

				std::string stringDelTablero = partida.TableroJugador.AString(false);

				Paquete paquete = {};
				paquete.Codigo = "2000";
				paquete.PreInput = "Nada por aqui.";
				paquete.TextoVisual = stringDelTablero.c_str();

				std::cout << "Enviado tablero" << std::endl;

				ssize_t valorLeido = -1;
				char bufferFlota[TAMANO_BUFFER] = {0};

				while (!partida.HaFinalizado()) {
					bool desconectar = partida.Iteracion(new_socket, matrizLED);
					std::cout << "Iteracion finalizada";
					if (desconectar) {
						std::cout << "Desconexion forzosa. Juego finalizado." << std::endl;
						break; // TODO: Desconectar correctamente, no con break
					}
				}

				// TODO: Guardar resultado

				if (partida.TableroJugador.CompletamenteHundido()) {
					std::cout << "Has perdido! No te quedan mas barcos. Suerte a la proxima!";
					mandarPaquete(new_socket, "Has perdido! No te quedan mas barcos. Suerte a la proxima!\n", "[ Pulsa una tecla para finalizar el juego ]", PULSACION, true);
				} else {
					std::cout << "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!";
					mandarPaquete(new_socket, "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!\n", "[ Pulsa una tecla para finalizar el juego ]", PULSACION, true);
				}

				leerDesdeCliente(new_socket); // Bloquear hasta respuesta
				logger.Log("Finalizado juego \"flota\".", CategoriaLog::Partida);

				break;
			} else if (accionElegida == '5') { // 4 en raya (vs CPU)

			} else if (accionElegida == '6') {
				sesion.SetMenuActual(TiposMenu::Menu1);
				// Devuelve a la pestaña anterior
			} else {
				// Error!
			}
			break;
		}
		case TiposMenu::Menu3: {
			char accionElegida = mensajeDeCliente.contenido[0];
			// estas opciones son para la eleccion de un juego a configurar
			if (accionElegida == '1') {

			} else if (accionElegida == '2') {

			} else if (accionElegida == '3') {

			} else if (accionElegida == '4') {

			} else if (accionElegida == '5') {

			} else if (accionElegida == '6') {
				sesion.SetMenuActual(TiposMenu::Menu1);
				// ddevuelve a la pestaña anterior
			} else {
				// Error!
			}
			break;
		}
		case TiposMenu::Menu0_Login: {
			std::string textoIntroducido = "";
			for (int i = 0; i < mensajeDeCliente.contenido.length(); i++) {
				textoIntroducido = textoIntroducido + mensajeDeCliente.contenido[i];
			}
			// char *textoIntroducido = (char *)malloc(strlen(bufferEntrante) * sizeof(char) + 1 * sizeof(char)); // porque? +1??
			//  for (int i = 0; i < strlen(bufferEntrante); i++) {
			//  	*(textoIntroducido + i) = *(bufferEntrante + i);
			//  }
			//  *(textoIntroducido + strlen(bufferEntrante)) = '\0';
			switch (sesion.GetEstadoLogin()) {
			case EstadosLoginRegistro::EsperandoUsuario: {
				sesion.SetNombreIntroducidoLoginRegistro(textoIntroducido);
				sesion.SetEstadoLogin(EstadosLoginRegistro::EsperandoContrasenya);
				break;
			}
			case EstadosLoginRegistro::EsperandoContrasenya: {
				int idUsuario = -1;
				bool correctas = CredencialesCorrectas(sesion.GetNombreIntroducidoLoginRegistro(), textoIntroducido, &idUsuario);
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
			std::string textoIntroducido = "";
			for (int i = 0; i < mensajeDeCliente.contenido.length(); i++) {
				textoIntroducido = textoIntroducido + mensajeDeCliente.contenido[i];
			}

			switch (sesion.GetEstadoLogin()) {
			case EstadosLoginRegistro::EsperandoUsuario: {
				sesion.SetNombreIntroducidoLoginRegistro(textoIntroducido);

				bool existe = VerUsuario(sesion.GetNombreIntroducidoLoginRegistro());
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

				bool correctas = CrearUsuario(sesion.GetNombreIntroducidoLoginRegistro(), textoIntroducido, &idUsuario);
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
		case TiposMenu::Menu4: { // Menu de estadisticas
			// Hay que cargar las estadisticas de un usuario en concreto de la bd
			printf("Tus estadisticas");

			break;
		}

		default: {
			printf("Input de menu no handleado.\n");

			break;
		}
		}

		Paquete paquete = CrearPaqueteDeMenu(sesion, ultimoError);
		std::string paqueteStr = paquete.AString();

		mandarPaquete(new_socket, paquete);
	}

	matrizLED->RellenarDeColor(ColorLED::Negro);

	std::cout << "Un cliente se ha desconectado. Cerrando servidor." << std::endl;
	logger.Log("Usuario desconectado. (\" " + sesion.GetNombreUsuario() + " \").", CategoriaLog::Desconexion);
	logger.Log("Cierre del servidor.", CategoriaLog::Otro);
	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	close(server_fd);
	return 0;
}
