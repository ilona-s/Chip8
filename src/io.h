#ifndef IO_H
#define IO_H

#include <SDL_events.h>
#include "chip8.h"

typedef struct c8_io_handler {
	SDL_Event event;
	bool quit;
} c8_io_handler;

c8_io_handler *c8_io_handler_init();

void c8_io_handler_quit(c8_io_handler *io);

void c8_handle_events(c8_io_handler *io, chip8 *c8);

#endif //IO_H
