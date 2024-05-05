#include "utilsLED.h"
#include "externo/rpi_ws281x/ws2811.h"
#include <cstdio>

using namespace utilsLED;

utilsLED::TiraLED::TiraLED() {
	int tipo = WS2811_STRIP_BGR;
	uint32_t frecuencia = WS2811_TARGET_FREQ;
	int pin = 18; // GPIO18
	int dma = 10;

	this->anchura = 8;
	this->altura = 8;
	this->tamanyo = this->anchura * this->altura;

	this->ledstring = {
		.freq = frecuencia,
		.dmanum = dma,
		.channel =
			{
				[0] =
					{
						.gpionum = pin,
						.invert = 0,
						.count = this->tamanyo,
						.strip_type = tipo,
						.brightness = 255,
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
}

utilsLED::TiraLED::~TiraLED() {
	free(this->matriz);
	ws2811_fini(&this->ledstring);
}

void utilsLED::TiraLED::Limpiar() { Colorear(utilsLED::ColorLED::Negro); }
void utilsLED::TiraLED::Colorear(ColorLED color) {
	int x, y;

	for (y = 0; y < altura; y++) {
		for (x = 0; x < anchura; x++) {
			this->matriz[y * anchura + x] = ValoresColoresLED[color];
		}
	}
}

void utilsLED::TiraLED::Renderizar() {
	int x, y;

	for (y = 0; y < altura; y++) {
		for (x = 0; x < anchura; x++) {
			ledstring.channel[0].leds[(y * anchura) + x] = matriz[y * anchura + x];
		}
	}

	ws2811_return_t ret;
	if ((ret = ws2811_render(&this->ledstring)) != WS2811_SUCCESS) {
		std::cout << "No se ha podido renderizar la matriz LED. Cerrando el servidor. Error: %s\n", ws2811_get_return_t_str(ret);
		exit(1);
	};
}
