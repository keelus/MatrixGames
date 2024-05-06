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
// #define STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define STRIP_TYPE WS2811_STRIP_GBR // WS2812/SK6812RGB integrated chip+leds
// #define STRIP_TYPE            SK6812_STRIP_RGBW		// SK6812RGBW (NOT SK6812RGB)

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

void matrix_setColor(void) {
	int x, y;

	// 0x00200000,  // red
	// 0x10200000,  // red + W
	// 0x00002000,  // green
	// 0x10002000,  // green + W
	// 0x00000020,  // blue
	// 0x10000020,  // blue + W
	// 0x00101010,  // white
	// 0x10101010,  // white + W

	for (y = 0; y < (height); y++) {
		for (x = 0; x < width; x++) {
			matrix[y * width + x] = 0x10002000;
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

int dotspos[] = {0, 1, 2, 3, 4, 5, 6, 7};
ws2811_led_t dotcolors[] = {
    0x00200000, // red
    0x00201000, // orange
    0x00202000, // yellow
    0x00002000, // green
    0x00002020, // lightblue
    0x00000020, // blue
    0x00100010, // purple
    0x00200010, // pink
};

ws2811_led_t dotcolors_rgbw[] = {
    0x00200000, // red
    0x10200000, // red + W
    0x00002000, // green
    0x10002000, // green + W
    0x00000020, // blue
    0x10000020, // blue + W
    0x00101010, // white
    0x10101010, // white + W

};

void matrix_bottom(void) {
	int i;

	for (i = 0; i < (int)(ARRAY_SIZE(dotspos)); i++) {
		dotspos[i]++;
		if (dotspos[i] > (width - 1)) {
			dotspos[i] = 0;
		}

		if (ledstring.channel[0].strip_type == SK6812_STRIP_RGBW) {
			matrix[dotspos[i] + (height - 1) * width] = dotcolors_rgbw[i];
		} else {
			matrix[dotspos[i] + (height - 1) * width] = dotcolors[i];
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
	ws2811_return_t ret;

	matrix = malloc(sizeof(ws2811_led_t) * width * height);

	setup_handlers();

	if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
		fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
		return ret;
	}

	printf("Set color");
	matrix_setColor();
	matrix_render();
	ws2811_render(&ledstring);

	sleep(5);

	printf("clear");
	matrix_clear();
	matrix_render();
	ws2811_render(&ledstring);

	sleep(2);

	if (clear_on_exit) {
		matrix_clear();
		matrix_render();
		ws2811_render(&ledstring);
	}

	ws2811_fini(&ledstring);

	printf("\n");
	return ret;
}
