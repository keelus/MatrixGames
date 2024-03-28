#include "menu.h"
#include "sql.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Menu mensajeMenu(TiposMenu menuActual, EstadosMenuLogin estadoLogin, const char *mensajeError, const char *usuario, const char *juegoSeleccionadoConf) {
	Menu menu = {};
	switch (menuActual) {
	case MENU_0: {
				

		if (mensajeError == NULL) {
			menu.TextoVisual = "\nSelecciona una opcion:\n\t1) Iniciar sesion\n\t2) Crear una cuenta\n\n\t3) Salir";
		} else {
			const char *textoVisual = "\nSelecciona una opcion:\n\t1) Iniciar sesion\n\t2) Crear una cuenta\n\n\t3) Salir";
			char *mensajeCombinado = (char *)malloc(strlen(mensajeError) + strlen(textoVisual) + 2);
			sprintf(mensajeCombinado, "\n%s%s", mensajeError, textoVisual);

			menu.TextoVisual = mensajeCombinado;
		}
		menu.PreInput = "\nOpcion seleccionada: ";
		menu.Codigo = "2000";
		

		break;
	}
	case MENU_0_LOGIN: {
		if (estadoLogin == ESPERANDO_USUARIO) {
			menu.TextoVisual = "== INICIAR SESION ==";
			menu.PreInput = "\nIntroduce tu nombre de usuario: ";
		} else if (estadoLogin == ESPERANDO_CONTRASENYA) {
			menu.TextoVisual = "";
			menu.PreInput = "Introduce tu contrasena: ";
		}
		menu.Codigo = "2000";

		break;
	}
	case MENU_1: {
		
		menu.TextoVisual = "\n\nHola,  !\n\nSelecciona una opcion:\n\t1) Jugar\n\t2) Configuracion\n\t3) Estadisticas\n\n\t4) Salir";
		menu.PreInput = "\nElige una de las opciones:  ";
		menu.Codigo = "2000";
		char *texto = (char *)malloc( (strlen(menu.TextoVisual) + strlen(usuario) + 2));

		sprintf(texto, "\n\nHola, %s !\n\nSelecciona una opcion:\n\t1) Jugar\n\t2) Configuracion\n\t3) Estadisticas\n\n\t4) Salir", usuario);
		menu.TextoVisual = texto;
		break;
		}
	case MENU_2: {
		
		menu.TextoVisual = "Selecciona un juego:\n\n\t1) Snake\n\t2)Flappy Bird\n\t3) Slip Grave\n\t4) Hundir la flota (vs CPU)\n\t5) 4 en raya (vs CPU)\n\n\t6) Volver atras";
		menu.PreInput = "\nElige una de las opciones:  ";
		menu.Codigo = "2000";
		char *texto = (char *)malloc( (strlen(menu.TextoVisual) + strlen(usuario) + 2));
		break;
		}
	case MENU_3: {
		
		menu.TextoVisual = "Selecciona el juego a configurar:\n\n\t1) Snake\n\t2)Flappy Bird\n\t3) Slip Grave\n\t4) Hundir la flota (vs CPU)\n\t5) 4 en raya (vs CPU)\n\n\t6) Volver atras";
		menu.PreInput = "\nElige una de las opciones:  ";
		menu.Codigo = "2000";
		char *texto = (char *)malloc( (strlen(menu.TextoVisual) + strlen(usuario) + 2));
		break;
		}
	
	case MENU_0_REGISTRO: {
		if(estadoLogin == ESPERANDO_USUARIO){
			menu.TextoVisual = "== REGISTRO ==";
			menu.PreInput = "\nIntroduce tu nombre de usuario: ";

		}else if (estadoLogin == ESPERANDO_CONTRASENYA) {
			menu.TextoVisual = "";
			menu.PreInput = "Introduce tu contrasena: ";
		}
			menu.Codigo = "2000";
		break;	
	}

	case CLOSE:{
			menu.TextoVisual = "CLOSE";
			menu.PreInput = " ";
			menu.Codigo = "0000";
			break;	
	}
	default: {
		printf("Usado un menu no definido. Cuidado.\n");
		menu.TextoVisual = "El menu actual no esta hecho.";
		menu.PreInput = "\nCierra el programa.";
		menu.Codigo = "2000";

		break;
	}
	
	}

	return menu;

}


const char *menuAString(Menu menu) {
	char *menuString = (char *)malloc(strlen(menu.TextoVisual) + strlen(menu.PreInput) + strlen(menu.Codigo) + 3 + 3 /*sumamos los ; y los \0*/);

	sprintf(menuString, "%s;%s;%s;", menu.Codigo, menu.TextoVisual, menu.PreInput);
	return menuString;
}
