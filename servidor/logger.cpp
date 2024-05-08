#include "logger.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Logger::Logger() {
	int dia, mes, anyo, hora, minuto, segundo;
	ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

	std::stringstream nombreArchivo;
	nombreArchivo << "log_";
	nombreArchivo << std::setw(2) << std::setfill('0') << dia << "-";
	nombreArchivo << std::setw(2) << std::setfill('0') << mes << "-";
	nombreArchivo << anyo << "_";
	nombreArchivo << std::setw(2) << std::setfill('0') << hora << "-";
	nombreArchivo << std::setw(2) << std::setfill('0') << minuto << "-";
	nombreArchivo << std::setw(2) << std::setfill('0') << segundo << ".txt";

	archivoLog.open("logs/" + nombreArchivo.str(), std::ios_base::app);
	if (!archivoLog.is_open()) {
		std::cout << "[!] Error al abrir el archivo log!" << std::endl;
	}
}

Logger::~Logger() {
	if (archivoLog.is_open())
		archivoLog.close();
}

void Logger::Log(std::string mensaje, CategoriaLog categoria) {
	int dia, mes, anyo, hora, minuto, segundo;
	ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

	std::string categoriaStr;
	switch (categoria) {
	case CategoriaLog::Conexion:
		categoriaStr = "Conexion";
		break;
	case CategoriaLog::Desconexion:
		categoriaStr = "Desconexion";
		break;
	case CategoriaLog::Insert:
		categoriaStr = "Insert";
		break;
	case CategoriaLog::Partida:
		categoriaStr = "Partida";
		break;
	case CategoriaLog::Otro:
		categoriaStr = "Otro";
		break;
	}

	std::stringstream mensajeFinal;
	mensajeFinal << "[";
	mensajeFinal << std::setw(2) << std::setfill('0') << dia << "/";
	mensajeFinal << std::setw(2) << std::setfill('0') << mes << "/";
	mensajeFinal << anyo << " - ";
	mensajeFinal << std::setw(2) << std::setfill('0') << hora << ":";
	mensajeFinal << std::setw(2) << std::setfill('0') << minuto << ":";
	mensajeFinal << std::setw(2) << std::setfill('0') << segundo << "]";
	mensajeFinal << "[" << categoriaStr << "] ";
	mensajeFinal << mensaje;

	archivoLog << mensajeFinal.str() << std::endl;
}

void Logger::ConseguirFecha(int &dia, int &mes, int &anyo, int &hora, int &minuto, int &segundo) {
	time_t t = std::time(0);
	std::tm *ahora = std::localtime(&t);

	dia = ahora->tm_mday;
	mes = ahora->tm_mon + 1;
	anyo = 2000 + ahora->tm_year % 100;

	hora = ahora->tm_hour;
	minuto = ahora->tm_min;
	segundo = ahora->tm_sec;
}
