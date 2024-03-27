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

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define TAMANO_BUFFER 512
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
	char *ultimoError = NULL;

	char *nombreUsuarioActual;

	Menu menu = mensajeMenu(menuActual, estadoLogin, ultimoError, nombreUsuarioActual, NULL);
	const char *menuAImprimir = menuAString(menu);

	iSendResult = send(ClientSocket, menuAImprimir, strlen(menuAImprimir), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("Error al mandar al cliente: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	char bufferEntrante[TAMANO_BUFFER];
	const char *database = "baseDeDatos.db";

	do {
		// Vaciar buffer de recepcion
		memset(bufferEntrante, '\0', TAMANO_BUFFER);

		iResult = recv(ClientSocket, bufferEntrante, TAMANO_BUFFER, 0);
		printf("Se ha recibido desde cliente: \"%s\"\n", bufferEntrante);

		// Aqui se eligue que hacer con el input (depende el menu, y el juego, etc)
		if (iResult > 0) {
			switch (menuActual) {
			case MENU_0: {
				char accionElegida = *bufferEntrante;

				if (accionElegida == '1') {
					menuActual = MENU_0_LOGIN;
				} else if (accionElegida == '2') {
					menuActual = MENU_0_REGISTRO;
				} else if (accionElegida == '3') {
					// Desconectar usuario
				} else {
					// Error!
				}

				break;
			}
			case MENU_0_LOGIN: {
				char *textoIntroducido = (char *)malloc(strlen(bufferEntrante) * sizeof(char) + 1 * sizeof(char));
				for (int i = 0; i < strlen(bufferEntrante); i++) {
					*(textoIntroducido + i) = *(bufferEntrante + i);
				}
				*(textoIntroducido + strlen(bufferEntrante)) = '\0';
				switch (estadoLogin) {
				case ESPERANDO_USUARIO: {
					nombreUsuarioActual = (char *)malloc(strlen(textoIntroducido) + 1);
					strcpy(nombreUsuarioActual, textoIntroducido);
					estadoLogin = ESPERANDO_CONTRASENYA;
					break;
				}
				case ESPERANDO_CONTRASENYA: {
					int idUsuario = -1;
					bool correctas = credencialesCorrectas(nombreUsuarioActual, textoIntroducido, database, &idUsuario);

					estadoLogin = ESPERANDO_USUARIO; // Reiniciamos estado para futuros logins
					if (correctas) {
						// Si es correcta, iniciamos sesion correctamente
						free(ultimoError); // Importante
						ultimoError = NULL;
						menuActual = MENU_1;
					} else {
						// Si no lo es, limpiamos usuario, y pasamos al menu 0.
						const char *mensajeError = "El usuario y/o contrasena son incorrectos. Intentalo de nuevo o crea una cuenta.";

						free(ultimoError); // Importante
						ultimoError = (char *)malloc((strlen(mensajeError) + 1) * sizeof(char));
						strcpy(ultimoError, mensajeError);

						menuActual = MENU_0;
						free(nombreUsuarioActual);
						nombreUsuarioActual = NULL;
					}
					break;
				}
				}

				free(textoIntroducido);
				textoIntroducido = NULL;
				break;
			}
			default: {
				printf("Input de menu no handleado.\n");
				break;
			}
			}

			Menu menu = mensajeMenu(menuActual, estadoLogin, ultimoError, nombreUsuarioActual, NULL);
			const char *menuAImprimir = menuAString(menu);

			iSendResult = send(ClientSocket, menuAImprimir, strlen(menuAImprimir), 0);
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
