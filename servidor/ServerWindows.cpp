#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501
#include "sql.h"
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define TAMANO_BUFFER 512
#define DEFAULT_PORT "3000"

int __cdecl main(void) {
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

	// No longer need server socket
	closesocket(ListenSocket);

	// Texto de ejemplo a mandar. Podriamos hacerlo de formato "codigo;menu, informacion o lo que sea a imprimir antes del input del cliente;mensaje preinput del cliente"
	const char *bufferSaliente = "3000;Bienvenido a MatrixGames, inicie sesion por favor;Digame el usuario;";

	// Mandar un mensaje de ejemplo, si no se quedara still en ambas partes
	iSendResult = send(ClientSocket, bufferSaliente, strlen(bufferSaliente), 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("Error al mandar al cliente: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	printf("Enviado.\n\n");
	int session = -1;
	char bufferEntrante[TAMANO_BUFFER];
	const  char *database = "baseDeDatos.db";
	do {
		// Vaciar buffer de recepcion
		memset(bufferEntrante, '\0', TAMANO_BUFFER);

		iResult = recv(ClientSocket, bufferEntrante, TAMANO_BUFFER, 0);
		if (iResult > 0) {
			printf("Se ha recibido desde cliente: \"%s\"\n", bufferEntrante);
			if(session == -1){
				// session == -1 significa que no ha iniciado sesion. en sesion guardaremos el id de usuario.
				
   				 int *pr = verificarUsuario(bufferEntrante,database);
				printf("ID de usuario: %d\n", *pr);
				session = *pr;
				 sprintf(bufferEntrante, "2000;usuario:%s dime la contrasena: ; ", bufferEntrante);
				
			}
			printf("Enviando mensaje de ejemplo...\n");
			if(bufferEntrante)

			// Echo the buffer back to the sender
			if(session !=-1){
				iSendResult = send(ClientSocket, bufferEntrante, strlen(bufferSaliente), 0);
			}else{
			iSendResult = send(ClientSocket, bufferSaliente, strlen(bufferSaliente), 0);

			}
			if (iSendResult == SOCKET_ERROR) {
				printf("Error al mandar al cliente: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Enviado.\n\n");
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
