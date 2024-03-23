#ifndef _INPUTH_
#define _INPUTH_

enum ModosEntrada { TEXTO, PULSACION };
typedef enum ModosEntrada ModosEntrada;

char *leerInput(ModosEntrada modo, char *textoVisual);
char *cambiarEntrada(int codigo); // Codigo de servidor recibido en el mensaje.

#endif
