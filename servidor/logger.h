#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>

enum class CategoriaLog { Conexion, Desconexion, Otro, Insert, Partida };

class Logger {
  public:
	Logger();
	~Logger();
	void Log(std::string mensaje, CategoriaLog categoria);

  private:
	std::ofstream archivoLog;
};

#endif
