#include "fecha.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

void Fecha::ConseguirFecha(int &dia, int &mes, int &anyo, int &hora, int &minuto, int &segundo) {
	time_t t = std::time(0);
	std::tm *ahora = std::localtime(&t);

	dia = ahora->tm_mday;
	mes = ahora->tm_mon + 1;
	anyo = 2000 + ahora->tm_year % 100;

	hora = ahora->tm_hour;
	minuto = ahora->tm_min;
	segundo = ahora->tm_sec;
}

std::string Fecha::ConseguirFechaVisual() {
	int dia, mes, anyo, hora, minuto, segundo;
	ConseguirFecha(dia, mes, anyo, hora, minuto, segundo);

	std::stringstream fechaFinal;
	fechaFinal << std::setw(2) << std::setfill('0') << dia << "/";
	fechaFinal << std::setw(2) << std::setfill('0') << mes << "/";
	fechaFinal << anyo << " - ";
	fechaFinal << std::setw(2) << std::setfill('0') << hora << ":";
	fechaFinal << std::setw(2) << std::setfill('0') << minuto << ":";
	fechaFinal << std::setw(2) << std::setfill('0') << segundo;

	return fechaFinal.str();
}
