#include "menu.h"
#include <cstring>
#include <string.h>
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include "menu.h"
#include "sql.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Flota
#include "juegos/flota/partida.h"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define TAMANO_BUFFER 1024
#define DEFAULT_PORT "3000"

int __cdecl main(void) {
	printf("__ __  __ _____ ___ ___   __ \n"
	       "|  V  |/  \\_   _| _ \\ \\ \\_/ / \n"
	       "| \\_/ | /\\ || | | v / |> , <  \n"
	       "|_|_|_|_||_||_| |_|_\\_/_/ \\_\\ \n"
	       " / _]/  \\|  V  | __/' _/      \n"
	       "| [/\\ /\\ | \\_/ | _|`._`.      \n"
	       " \\__/_||_|_| |_|___|___/\n"
	       "  ====  SERVIDOR  ====  \n");

	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	printf("Un cliente se ha conectado.\n");

	closesocket(ListenSocket); // ??

	TiposMenu menuActual = MENU_0;
	EstadosMenuLogin estadoLogin = ESPERANDO_USUARIO;
	std::string ultimoError = "";

	std::string nombreUsuarioActual = "";

	Paquete paquete = CrearPaqueteDeMenu(menuActual, estadoLogin, ultimoError, nombreUsuarioActual, "");
	std::string paqueteStr = paquete.AString();

	iSendResult = send(ClientSocket, paqueteStr.c_str(), paqueteStr.length(), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("Error al mandar al cliente: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	char bufferEntrante[TAMANO_BUFFER];

	do {
		// Vaciar buffer de recepcion
		memset(bufferEntrante, '\0', TAMANO_BUFFER);

		iResult = recv(ClientSocket, bufferEntrante, TAMANO_BUFFER, 0);
		printf("Se ha recibido desde cliente: \"%s\"\n", bufferEntrante);

		// Aqui se eligue que hacer con el input (depende el menu, y el juego, etc)
		if (iResult > 0) {
			switch (menuActual) {
			case MENU_0: {
				char accionElegida = bufferEntrante[0];

				if (accionElegida == '1') {
					menuActual = MENU_0_LOGIN;
				} else if (accionElegida == '2') {
					menuActual = MENU_0_REGISTRO;
				} else if (accionElegida == '3') {
					// Desconectar usuario
					menuActual = CLOSE;
				} else {
					// Error!
				}

				break;
			}
			case MENU_1: {
				char accionElegida = *bufferEntrante;
				if (accionElegida == '1') {
					menuActual = MENU_2;
				} else if (accionElegida == '2') {
					menuActual = MENU_3;
				} else if (accionElegida == '3') {

				} else if (accionElegida == '4') {
					// Desconectar usuario
					menuActual = CLOSE;
				} else {
					// Error!
				}
				break;
			}
			case MENU_2: {
				char accionElegida = *bufferEntrante;
				// estas opciones son para iniciar los juesgos, menos el 6 que es para ir al menu anterior
				if (accionElegida == '1') { // Snake

				} else if (accionElegida == '2') { // Flappy Bird

				} else if (accionElegida == '3') { // Slip Grave

				} else if (accionElegida == '4') { // Hundir la flota (vs CPU)
					flota::Partida partida;

					// Prueba: Mandar tablero al jugador

					std::string stringDelTablero = partida.TableroJugador.AString(false);

					Paquete paquete = {};
					paquete.Codigo = "2000";
					paquete.PreInput = "Nada por aqui.";
					paquete.TextoVisual = stringDelTablero.c_str();

					std::string paqueteStr = paquete.AString();

					iSendResult = send(ClientSocket, paqueteStr.c_str(), paqueteStr.length(), 0);
					if (iSendResult == SOCKET_ERROR) {
						printf("Error al mandar al cliente: %d\n", WSAGetLastError());
						closesocket(ClientSocket);
						WSACleanup();
						return 1;
					}

					exit(1);

					while (!partida.HaFinalizado()) {
						partida.Iteracion();
					}

					if (partida.TableroJugador.CompletamenteHundido()) {
						std::cout << "Has perdido! No te quedan mas barcos. Suerte a la proxima!";
					} else {
						std::cout << "Has ganado! A la CPU no le quedan mas barcos. Bien hecho!";
					}

					exit(1);
				} else if (accionElegida == '5') { // 4 en raya (vs CPU)

				} else if (accionElegida == '6') {
					menuActual = MENU_1;
					// Devuelve a la pestaña anterior
				} else {
					// Error!
				}
				break;
			}
			case MENU_3: {
				char accionElegida = *bufferEntrante;
				// estas opciones son para la eleccion de un juego a configurar
				if (accionElegida == '1') {

				} else if (accionElegida == '2') {

				} else if (accionElegida == '3') {

				} else if (accionElegida == '4') {

				} else if (accionElegida == '5') {

				} else if (accionElegida == '6') {
					menuActual = MENU_1;
					// ddevuelve a la pestaña anterior
				} else {
					// Error!
				}
				break;
			}
			case MENU_0_LOGIN: {
				std::string textoIntroducido = "";
				for (int i = 0; i < strlen(bufferEntrante); i++) {
					textoIntroducido = textoIntroducido + bufferEntrante[i];
				}
				// char *textoIntroducido = (char *)malloc(strlen(bufferEntrante) * sizeof(char) + 1 * sizeof(char)); // porque? +1??
				//  for (int i = 0; i < strlen(bufferEntrante); i++) {
				//  	*(textoIntroducido + i) = *(bufferEntrante + i);
				//  }
				//  *(textoIntroducido + strlen(bufferEntrante)) = '\0';
				switch (estadoLogin) {
				case ESPERANDO_USUARIO: {
					nombreUsuarioActual = textoIntroducido;
					estadoLogin = ESPERANDO_CONTRASENYA;
					break;
				}
				case ESPERANDO_CONTRASENYA: {
					int idUsuario = -1;
					bool correctas = credencialesCorrectas(nombreUsuarioActual, textoIntroducido, &idUsuario);

					estadoLogin = ESPERANDO_USUARIO; // Reiniciamos estado para futuros logins
					if (correctas) {
						// Si es correcta, iniciamos sesion correctamente
						ultimoError = "";
						menuActual = MENU_1;
					} else {
						// Si no lo es, limpiamos usuario, y pasamos al menu 0.
						std::string mensajeError = "El usuario y/o contrasena son incorrectos. Intentalo de nuevo o crea una cuenta.";

						ultimoError = mensajeError;

						menuActual = MENU_0;
						nombreUsuarioActual = "";
					}
					break;
				}
				}
				break;
			}
			case MENU_0_REGISTRO: {
				std::string textoIntroducido = "";
				for (int i = 0; i < strlen(bufferEntrante); i++) {
					textoIntroducido = textoIntroducido + bufferEntrante[i];
				}

				switch (estadoLogin) {
				case ESPERANDO_USUARIO: {
					nombreUsuarioActual = textoIntroducido;

					bool existe = verUsuario(nombreUsuarioActual);
					if (existe) {
						std::string mensajeError = "Este usuario ya existe, por favor cree la cuenta con otro nombre de usuario.";
						ultimoError = mensajeError;
						menuActual = MENU_0;
						nombreUsuarioActual = "";
						break;
					}
					estadoLogin = ESPERANDO_CONTRASENYA;
					break;
				}
				case ESPERANDO_CONTRASENYA: {
					int idUsuario = -1;
					bool correctas = crearUsuario(nombreUsuarioActual, textoIntroducido, &idUsuario);
					printf("id de usuario%i \n", idUsuario);
					if (correctas) {
						ultimoError = "";
						menuActual = MENU_1;
					} else {
						std::string mensajeError = "Este usuario ya existe, por favor cree la cuenta con otro nombre de usuario.";
						ultimoError = mensajeError;

						menuActual = MENU_0;

						nombreUsuarioActual = "";
						estadoLogin = ESPERANDO_USUARIO;
					}
					break;
				}
				}
				break;
			}
			case MENU_4: { // Menu de estadisticas
				// Hay que cargar las estadisticas de un usuario en concreto de la bd
				printf("Tus estadisticas");

				break;
			}

			default: {
				printf("Input de menu no handleado.\n");
				printf("%i", menuActual);

				break;
			}
			}

			Paquete paquete = CrearPaqueteDeMenu(menuActual, estadoLogin, ultimoError, nombreUsuarioActual, "");
			std::string paqueteStr = paquete.AString();

			iSendResult = send(ClientSocket, paqueteStr.c_str(), paqueteStr.length(), 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("Error al mandar al cliente: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
		} else if (iResult == 0)
			printf("Cerrando conexion...\n");
		else {
			printf("recv falla con error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}
	} while (iResult > 0);
	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}
