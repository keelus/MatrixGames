#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "version.h"

#include "ws2811.h"

#define ARRAY_SIZE(stuff) (sizeof(stuff) / sizeof(stuff[0]))

// defaults for cmdline options
#define TARGET_FREQ WS2811_TARGET_FREQ
#define GPIO_PIN 18
#define DMA 10
#define STRIP_TYPE WS2811_STRIP_GRB

#define WIDTH 8
#define HEIGHT 8
#define LED_COUNT (WIDTH * HEIGHT)

int width = WIDTH;
int height = HEIGHT;
int led_count = LED_COUNT;

int clear_on_exit = 0;

ws2811_t ledstring = {
	.freq = TARGET_FREQ,
	.dmanum = DMA,
	.channel =
		{
			[0] =
				{
					.gpionum = GPIO_PIN,
					.invert = 0,
					.count = LED_COUNT,
					.strip_type = STRIP_TYPE,
					.brightness = 10,
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

ws2811_led_t *matrix;

static uint8_t running = 1;

void matrix_render(void) {
	int x, y;

	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			ledstring.channel[0].leds[(y * width) + x] = matrix[y * width + x];
		}
	}
}

void matrix_setColor(unsigned int valorHex) {
	int x, y;

	for (y = 0; y < (height); y++) {
		for (x = 0; x < width; x++) {
			matrix[y * width + x] = valorHex;
		}
	}
}

void matrix_raise(void) {
	int x, y;

	for (y = 0; y < (height - 1); y++) {
		for (x = 0; x < width; x++) {
			// This is for the 8x8 Pimoroni Unicorn-HAT where the LEDS in subsequent
			// rows are arranged in opposite directions
			matrix[y * width + x] = matrix[(y + 1) * width + width - x - 1];
		}
	}
}

void matrix_clear(void) {
	int x, y;

	for (y = 0; y < (height); y++) {
		for (x = 0; x < width; x++) {
			matrix[y * width + x] = 0;
		}
	}
}

static void ctrl_c_handler(int signum) {
	(void)(signum);
	running = 0;
}

static void setup_handlers(void) {
	struct sigaction sa = {
		.sa_handler = ctrl_c_handler,
	};

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Uso: ./probarColor 0x00RRGGBB (color hex)");
		exit(1);
	}

	unsigned int valorHex;
	sscanf(argv[1], "%x", &valorHex);

	ws2811_return_t ret;

	matrix = malloc(sizeof(ws2811_led_t) * width * height);

	setup_handlers();

	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}

	printf("Establecer color a 0x%08X\n", valorHex);
	matrix_setColor(valorHex);
	matrix_render();
	ws2811_render(&ledstring);

	printf("Apagando en 3 segundos\n");
	sleep(3);

	matrix_clear();
	matrix_render();
	ws2811_render(&ledstring);

	ws2811_fini(&ledstring);

	printf("\n");
	return ret;
}
