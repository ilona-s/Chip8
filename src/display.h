#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include "chip8.h"

typedef struct c8_display {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} c8_display;

c8_display *c8_display_init();

void c8_display_quit(c8_display *display);

void c8_render_graphics(c8_display *display, chip8 *c8);

#endif //DISPLAY_H
