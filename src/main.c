#include <stdio.h>
#include <SDL.h>

#include "chip8.h"
#include "display.h"
#include "io.h"

int main(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: chip8 path_to_rom [clock_speed]\n");
		exit(1);
	}

	char *path_to_rom = argv[1];
	int clock_speed = (argc == 3) ? strtol(argv[2], NULL, 10) : 500; // Default clock speed 500Hz


	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		exit(1);
	}

	chip8 *c8 = chip8_init();
	c8_display *display = c8_display_init();
	c8_io_handler *io = c8_io_handler_init();

	chip8_load_ROM(c8, path_to_rom, 0x200);

	uint32_t timer = SDL_GetTicks();
	while (!io->quit) {
		c8_handle_events(io, c8);

		if (SDL_GetTicks() - timer > ((float) 1 / (float) clock_speed) * 1000) {
			timer = SDL_GetTicks();

			uint16_t opcode = chip8_get_opcode(c8);
			chip8_step(c8, opcode);

			if (c8->draw_flag) {
				c8_render_graphics(display, c8);
			}
		}
	}

	chip8_quit(c8);
	c8_display_quit(display);
	c8_io_handler_quit(io);

	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	return 0;
}