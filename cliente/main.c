#define WIN32_LEAN_AND_MEAN

#include "input.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

#define TAMANO_BUFFER 512
#define PUERTO "3000"
#define IP "localhost"

int __cdecl main() {
	ModosEntrada modoDeEntrada = PULSACION;

	WSADATA wsaData;
	SOCKET socketConexion = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	char recvbuf[TAMANO_BUFFER];
	int iResult;
	int recvbuflen = TAMANO_BUFFER;

	// Inicializar WINSOCK
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("Winsock falla con error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(IP, PUERTO, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo falla con error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server
		socketConexion = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (socketConexion == INVALID_SOCKET) {
			printf("socket falla con error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(socketConexion, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(socketConexion);
			socketConexion = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (socketConexion == INVALID_SOCKET) {
		printf("Error al conectar con el servidor!\n");
		WSACleanup();
		return 1;
	}

	// Bucle principal
	char *textoAMostrar = "Introduce algo: "; // Esto luego cambiara con la respuesta del servidor
	while (1) {
		char *entrada = leerInput(modoDeEntrada, textoAMostrar);
		iResult = send(socketConexion, entrada, (int)strlen(entrada), 0);

		if (iResult == SOCKET_ERROR) {
			printf("fallo al mandar. error: %d\n", WSAGetLastError());
			closesocket(socketConexion);
			WSACleanup();
			return 1;
		}

		free(entrada);
	}
	free(textoAMostrar);

	// Cerrar la conexion
	iResult = shutdown(socketConexion, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(socketConexion);
		WSACleanup();
		return 1;
	}

	// Esperar hasta que la conexion se cierre desde servidor
	do {

		iResult = recv(socketConexion, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// Limpieza
	closesocket(socketConexion);
	WSACleanup();

	return 0;
}
