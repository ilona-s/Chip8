#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_REGS 16
#define NUM_KEYS 16

#define STK_SIZE 16
#define MEM_SIZE 4096

#define GFX_WIDTH 64
#define GFX_HEIGHT 32

typedef struct chip8 {
	uint16_t PC;
	uint16_t I; // Address register

	uint16_t SP;
	uint16_t stack[STK_SIZE];

	uint8_t V[NUM_REGS];
	uint8_t key[NUM_KEYS];

	uint8_t memory[MEM_SIZE];

	uint8_t delay_timer;
	uint8_t sound_timer;

	bool draw_flag;
	uint32_t graphics[GFX_WIDTH * GFX_HEIGHT];
} chip8;

chip8 *chip8_init();

void chip8_quit(chip8 *c8);

void chip8_load_ROM(chip8 *c8, char *path, int offset);

uint16_t chip8_get_opcode(chip8 *c8);

void chip8_step(chip8 *c8, uint16_t opcode);

#endif //CHIP8_H
