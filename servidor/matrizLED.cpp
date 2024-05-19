#include "matrizLED.h"
#include "colorLED.h"
#include "externo/rpi_ws281x/ws2811.h"
#include "juegos/flota/casilla.h"

MatrizLED::MatrizLED(bool usandoRaspberry) { // Debera de ser false, si se esta compilando en Linux, cuando no esta la LED conectada o no se esta usando la Raspberry. Si no, no se podra usar el programa.
	std::cout << "###### LLAMADO A CONSTRUCTOR MATRIZ LED ######" << std::endl;
	this->UsandoRaspberry = usandoRaspberry;

	if (usandoRaspberry) {
		int tipo = WS2811_STRIP_GRB;
		uint32_t frecuencia = WS2811_TARGET_FREQ;
		int pin = 18; // GPIO18
		int dma = 10;

		this->ledstring = {
			.freq = frecuencia,
			.dmanum = dma,
			.channel =
				{
					[0] =
						{
							.gpionum = pin,
							.invert = 0,
							.count = int(this->tamanyo),
							.strip_type = tipo,
							.brightness = 10, // O poner una resistencia
						},
					[1] =
						{
							.gpionum = 0,
							.invert = 0,
							.count = 0,
							.brightness = 0,
						},
				},
		};

		this->matriz = (ws2811_led_t *)malloc(sizeof(ws2811_led_t) * this->anchura * this->altura);

		ws2811_return_t ret;
		if ((ret = ws2811_init(&this->ledstring)) != WS2811_SUCCESS) {
			std::cout << "Error al inizializar la matriz LED. Cerrando servidor. Error generado: " << ws2811_get_return_t_str(ret) << std::endl;
			;
			exit(1);
		}
	}

	this->RellenarDeColor(ColorLED::Negro);
}

MatrizLED::~MatrizLED() {
	free(this->matriz);
	ws2811_fini(&this->ledstring);
}

void MatrizLED::SetPixel(unsigned int x, unsigned int y, ColorLED colorLED) {
	if (x < 8 && y < 8) {
		this->MatrizColor[y][x] = colorLED;
	} else {
		std::cout << "Error al poner el pixel de color. Coordenadas fuera de rango (x: " << x << ", y: " << y << ")" << std::endl;
		exit(1);
	}

	this->dibujar();
}

ColorLED MatrizLED::GetPixel(unsigned int x, unsigned int y) {
	if (x < 8 && y < 8) {
		return this->MatrizColor[y][x];
	} else {
		std::cout << "Error al conseguir el pixel. Coordenadas fuera de rango (x: " << x << ", y: " << y << ")" << std::endl;
		exit(1);
	}
}

void MatrizLED::SetMatrizColor(ColorLED nuevoContenido[8][8]) {
	for (unsigned int y = 0; y < this->altura; y++) {
		for (unsigned int x = 0; x < this->anchura; x++) {
			this->MatrizColor[y][x] = nuevoContenido[y][x];
		}
	}

	this->dibujar();
}

void MatrizLED::RellenarDeColor(ColorLED colorLED) {
	for (unsigned int y = 0; y < this->altura; y++) {
		for (unsigned int x = 0; x < this->anchura; x++) {
			this->MatrizColor[y][x] = colorLED;
		}
	}

	this->dibujar();
}

void MatrizLED::Limpiar() { return this->RellenarDeColor(ColorLED::Negro); }

void MatrizLED::ImprimirMatrizColor() {
	std::string numeros[] = {u8"❶", u8"❷", u8"❸", u8"❹", u8"❺", u8"❻", u8"❼", u8"❽"};

	std::cout << "\n  🅰  🅱  🅲  🅳  🅴  🅵  🅶  🅷 \n";
	for (int i = 0; i < 8; ++i) {
		std::cout << numeros[8 - i - 1] << " ";
		for (int j = 0; j < 8; ++j) {
			std::string s = ColorString(this->MatrizColor[i][j]);
			std::cout << " " << s << " ";
		}
		std::cout << std::endl;
	}
}

void MatrizLED::dibujar() {
	if (!this->UsandoRaspberry)
		return;

	int valorColor;
	for (unsigned int y = 0; y < this->altura; y++) {
		for (unsigned int x = 0; x < this->anchura; x++) {
			valorColor = static_cast<int>(this->MatrizColor[y][x]);

			if (x % 2 != 0) {
				this->matriz[x * this->anchura + (this->anchura - 1) - y] = valorColor;
			} else {
				this->matriz[x * this->anchura + y] = valorColor;
			}
		}
	}

	this->renderizar();
}

void MatrizLED::renderizar() {
	int x, y;

	for (y = 0; y < altura; y++) {
		for (x = 0; x < anchura; x++) {
			ledstring.channel[0].leds[(y * anchura) + x] = matriz[y * anchura + x];
		}
	}

	ws2811_return_t ret;
	if ((ret = ws2811_render(&this->ledstring)) != WS2811_SUCCESS) {
		std::cout << "No se ha podido renderizar la matriz LED. Cerrando el servidor. Error generado: %s\n" << ws2811_get_return_t_str(ret) << std::endl;
		exit(1);
	};
}
