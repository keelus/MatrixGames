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
	struct entrantes
	{
		int codigo;
		char menu[200];
		char peticion[200];
	};
	
	ModosEntrada modoDeEntrada = PULSACION;
	WSADATA wsaData;
	SOCKET socketConexion = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	char bufferEntrante[TAMANO_BUFFER];
	char *bufferSaliente;
	int iResult;

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

	char *textoAMostrar = "Introduce algo: "; // Esto luego cambiara con la respuesta del servidor
	
	struct entrantes datos;
	// Bucle principal
	do {
		iResult = recv(socketConexion, bufferEntrante, TAMANO_BUFFER, 0);
		if (iResult > 0) {
			// Aqui es donde tendriamos que leer lo que recibimos desde el servidor.
			// Ver el codigo, texto... del mensaje y hacer algo dependiendo este
			const char *delimiter = ";";
			struct entrantes datos = {0, "", ""};
			// Inicializar el puntero de token
			char *token = strtok(bufferEntrante, delimiter);

			// Contador para llevar el seguimiento de los tokens
			int contador = 0;

			// Mientras haya tokens disponibles y no se haya alcanzado el límite de elementos en la estructura
			while (token != NULL && contador < MAX_NUM_REASONS) {
					// Procesar el token actual y guardarlo en la estructura 'datos'
				switch (contador) {
					case 0:
						datos.codigo = atoi(token);
						break;
					case 1:
						strncpy(datos.menu, token, sizeof(datos.menu) - 1);
						datos.menu[sizeof(datos.menu) - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
						break;
					case 2:
						strncpy(datos.peticion, token, sizeof(datos.peticion) - 1);
						datos.peticion[sizeof(datos.peticion) - 1] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
						break;
					}
					contador++;
					token = strtok(NULL, delimiter);
				}
		

			// if(codigo == 3000") por ejemplo pues queremos texto
			if(datos.codigo == 2000){
				modoDeEntrada = TEXTO;
				
			}
			

			if (datos.codigo == 3000){
				modoDeEntrada=TEXTO;
				
			}
			
			char *textoAMostrar = datos.peticion;
			printf("%s\n", datos.menu );
			bufferSaliente = leerInput(modoDeEntrada, textoAMostrar);

			iResult = send(socketConexion, bufferSaliente, TAMANO_BUFFER, 0);

			if (iResult == SOCKET_ERROR) {
				printf("fallo al mandar. error: %d\n", WSAGetLastError());
				closesocket(socketConexion);
				WSACleanup();
				return 1;
			}

			free(bufferSaliente);
			bufferSaliente = NULL;
		} else if (iResult == 0)
			printf("Conexion cerrada\n");
		else
			printf("recv falla with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// Cerrar la conexion
	iResult = shutdown(socketConexion, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(socketConexion);
		WSACleanup();
		return 1;
	}

	// Limpieza
	closesocket(socketConexion);
	WSACleanup();

	return 0;
}
