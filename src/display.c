#include "display.h"

c8_display *c8_display_init() {
	c8_display *display = malloc(sizeof(c8_display));
	if (!display) {
		perror("malloc");
		exit(1);
	}

	display->window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                   GFX_WIDTH * 10, GFX_HEIGHT * 10,
	                                   SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
	if (!display->window) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		exit(1);
	}

	display->renderer = SDL_CreateRenderer(display->window, -1, 0);
	if (!display->renderer) {
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		exit(1);
	}

	display->texture = SDL_CreateTexture(display->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
						GFX_WIDTH, GFX_HEIGHT);
	if (!display->texture) {
		fprintf(stderr, "SDL_CreateTexture: %s\n", SDL_GetError());
		exit(1);
	}

	return display;
}

void c8_display_quit(c8_display *display) {
	SDL_DestroyTexture(display->texture);
	SDL_DestroyRenderer(display->renderer);
	SDL_DestroyWindow(display->window);

	free(display);
}

void c8_render_graphics(c8_display *display, chip8 *c8) {
	SDL_UpdateTexture(display->texture, NULL, c8->graphics, GFX_WIDTH * sizeof(uint32_t));

	SDL_RenderSetScale(display->renderer, 10, 10);
	SDL_RenderCopy(display->renderer, display->texture, NULL, NULL);
	SDL_RenderPresent(display->renderer);

	c8->draw_flag = false;
}