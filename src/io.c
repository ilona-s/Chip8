#include "io.h"

c8_io_handler *c8_io_handler_init() {
	c8_io_handler *io = malloc(sizeof(c8_io_handler));
	if (!io) {
		perror("malloc");
		exit(1);
	}

	io->quit = false;

	return io;
}

void c8_io_handler_quit(c8_io_handler *io) {
	free(io);
}

void c8_handle_key_down(chip8 *c8, SDL_Keycode key) {
	switch (key) {
		case (SDLK_x):
			c8->key[0x0] = 1;
			break;
		case (SDLK_1):
			c8->key[0x1] = 1;
			break;
		case (SDLK_2):
			c8->key[0x2] = 1;
			break;
		case (SDLK_3):
			c8->key[0x3] = 1;
			break;
		case (SDLK_q):
			c8->key[0x4] = 1;
			break;
		case (SDLK_w):
			c8->key[0x5] = 1;
			break;
		case (SDLK_e):
			c8->key[0x6] = 1;
			break;
		case (SDLK_a):
			c8->key[0x7] = 1;
			break;
		case (SDLK_s):
			c8->key[0x8] = 1;
			break;
		case (SDLK_d):
			c8->key[0x9] = 1;
			break;
		case (SDLK_z):
			c8->key[0xA] = 1;
			break;
		case (SDLK_c):
			c8->key[0xB] = 1;
			break;
		case (SDLK_4):
			c8->key[0xC] = 1;
			break;
		case (SDLK_r):
			c8->key[0xD] = 1;
			break;
		case (SDLK_f):
			c8->key[0xE] = 1;
			break;
		case (SDLK_v):
			c8->key[0xF] = 1;
			break;
		default:
			break;
	}
}

void c8_handle_key_up(chip8 *c8, SDL_Keycode key) {
	switch (key) {
		case (SDLK_x):
			c8->key[0x0] = 0;
			break;
		case (SDLK_1):
			c8->key[0x1] = 0;
			break;
		case (SDLK_2):
			c8->key[0x2] = 0;
			break;
		case (SDLK_3):
			c8->key[0x3] = 0;
			break;
		case (SDLK_q):
			c8->key[0x4] = 0;
			break;
		case (SDLK_w):
			c8->key[0x5] = 0;
			break;
		case (SDLK_e):
			c8->key[0x6] = 0;
			break;
		case (SDLK_a):
			c8->key[0x7] = 0;
			break;
		case (SDLK_s):
			c8->key[0x8] = 0;
			break;
		case (SDLK_d):
			c8->key[0x9] = 0;
			break;
		case (SDLK_z):
			c8->key[0xA] = 0;
			break;
		case (SDLK_c):
			c8->key[0xB] = 0;
			break;
		case (SDLK_4):
			c8->key[0xC] = 0;
			break;
		case (SDLK_r):
			c8->key[0xD] = 0;
			break;
		case (SDLK_f):
			c8->key[0xE] = 0;
			break;
		case (SDLK_v):
			c8->key[0xF] = 0;
			break;
		default:
			break;
	}
}

void c8_handle_events(c8_io_handler *io, chip8 *c8) {
	while (SDL_PollEvent(&io->event)) {
		if (io->event.type == SDL_QUIT) {
			io->quit = true;
		} else if (io->event.type == SDL_KEYDOWN) {
			c8_handle_key_down(c8, io->event.key.keysym.sym);
		} else if (io->event.type == SDL_KEYUP) {
			c8_handle_key_up(c8, io->event.key.keysym.sym);
		}
	}
}
