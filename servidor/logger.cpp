#include "logger.h"
#include "fecha.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Logger::Logger() {
	int dia, mes, anyo, hora, minuto, segundo;
	Fecha::ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

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
	Fecha::ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

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
	mensajeFinal << "[" << Fecha::ConseguirFechaVisual() << "]";
	mensajeFinal << "[" << categoriaStr << "] ";
	mensajeFinal << mensaje;

	archivoLog << mensajeFinal.str() << std::endl;
}
