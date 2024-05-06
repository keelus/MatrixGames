#include "utilsLED.h"
#include "colorLED.h"
#include "externo/rpi_ws281x/ws2811.h"
#include "matrizColor.h"

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
		std::cout << "Error al inizializar la tira LED. Cerrando servidor. Error generado: " << ws2811_get_return_t_str(ret) << std::endl;
		;
		exit(1);
	}
}

utilsLED::TiraLED::~TiraLED() {
	free(this->matriz);
	ws2811_fini(&this->ledstring);
}

void utilsLED::TiraLED::Colorear(MatrizColor matrizColor) {
	int valorColor;
	for (unsigned int y = 0; y < this->altura; y++) {
		for (unsigned int x = 0; x < this->anchura; x++) {
			valorColor = static_cast<int>(matrizColor.getPixel(x, y));
			this->matriz[y * anchura + x] = valorColor;
		}
	}

	this->renderizar();
}

void utilsLED::TiraLED::renderizar() {
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
