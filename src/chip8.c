#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chip8.h"

#define HIGH_NIBBLE(WORD) ((WORD & 0xF000) >> 12)
#define LOW_NIBBLE(WORD) (WORD & 0xF)

#define HIGH_BYTE(WORD) ((WORD & 0xFF00) >> 8)
#define LOW_BYTE(WORD) (WORD & 0xFF)

#define IMMEDIATE(WORD) (WORD & 0xFF)
#define ADDRESS(WORD) (WORD & 0xFFF)

#define X_REG(WORD) ((WORD & 0xF00) >> 8)
#define Y_REG(WORD) ((WORD & 0xF0) >> 4)

uint8_t font_set[] = {0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
                      0x20, 0x60, 0x20, 0x20, 0x70,		// 1
                      0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
                      0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
                      0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
                      0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
                      0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
                      0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
                      0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
                      0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
                      0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
                      0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
                      0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
                      0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
                      0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
                      0xF0, 0x80, 0xF0, 0x80, 0x80};	        // F

chip8 *chip8_init() {
	chip8 *c8 = malloc(sizeof(chip8));
	if (!c8) {
		perror("malloc");
		exit(1);
	}

	c8->PC = 0x200;
	c8->I = 0;
	c8->SP = 0;

	memset(c8->V, 0, sizeof(c8->V));
	memset(c8->key, 0, sizeof(c8->key));
	memset(c8->stack, 0, sizeof(c8->stack));

	memset(c8->memory, 0, sizeof(c8->memory));
	memcpy(c8->memory, font_set, sizeof(font_set));

	memset(c8->graphics, 0, sizeof(c8->graphics));
	c8->draw_flag = false;

	c8->sound_timer = 0;
	c8->delay_timer = 0;

	return c8;
}

void chip8_quit(chip8 *c8) {
	free(c8);
}

void chip8_load_ROM(chip8 *c8, char *path, int offset) {
	FILE *file = fopen(path, "rb");
	if (!file) {
		perror("fopen");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	uint32_t fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (fsize + offset > MEM_SIZE) {
		fprintf(stderr, "File too large\n");
		exit(1);
	}

	if (fread(c8->memory + offset, 1, fsize, file) != fsize) {
		perror("fread");
		exit(1);
	}

	if (fclose(file) < 0) {
		perror("fclose");
		exit(1);
	}
}

uint16_t chip8_get_opcode(chip8 *c8) {
	uint16_t opcode = c8->memory[c8->PC] << 8 | c8->memory[c8->PC + 1];
	c8->PC += 2;

	return opcode;
}

void chip8_clear_screen(chip8 *c8) {
	memset(c8->graphics, 0, sizeof(c8->graphics));
	c8->draw_flag = true;
}

void chip8_return(chip8 *c8) {
	c8->PC = c8->stack[--c8->SP];
}

void chip8_jump(chip8 *c8, uint16_t opcode) {
	c8->PC = ADDRESS(opcode);
}

void chip8_jump_plus_r0(chip8 *c8, uint16_t opcode) {
	c8->PC = ADDRESS(opcode) + c8->V[0];
}

void chip8_call(chip8 *c8, uint16_t opcode) {
	c8->stack[c8->SP++] = c8->PC;
	c8->PC = ADDRESS(opcode);
}

void chip8_skip_eq_imm(chip8 *c8, uint16_t opcode) {
	if (c8->V[X_REG(opcode)] == IMMEDIATE(opcode)) {
		c8->PC += 2;
	}
}

void chip8_skip_neq_imm(chip8 *c8, uint16_t opcode) {
	if (c8->V[X_REG(opcode)] != IMMEDIATE(opcode)) {
		c8->PC += 2;
	}
}

void chip8_skip_eq_reg(chip8 *c8, uint16_t opcode) {
	if (c8->V[X_REG(opcode)] == c8->V[Y_REG(opcode)]) {
		c8->PC += 2;
	}
}

void chip8_skip_neq_reg(chip8 *c8, uint16_t opcode) {
	if (c8->V[X_REG(opcode)] != c8->V[Y_REG(opcode)]) {
		c8->PC += 2;
	}
}

void chip8_set_imm(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] = IMMEDIATE(opcode);
}

void chip8_add_imm(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] += IMMEDIATE(opcode);
}

void chip8_set_reg(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] = c8->V[Y_REG(opcode)];
}

void chip8_or_reg(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] |= c8->V[Y_REG(opcode)];
}

void chip8_and_reg(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] &= c8->V[Y_REG(opcode)];
}

void chip8_xor_reg(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] ^= c8->V[Y_REG(opcode)];
}

void chip8_add_reg(chip8 *c8, uint16_t opcode) {
	uint16_t sum16 = c8->V[X_REG(opcode)] + c8->V[Y_REG(opcode)];
	uint8_t sum8 = sum16 & 0xFF;

	c8->V[0xF] = (sum16 & 0x100) ? 1 : 0; // VF reflects carry
	c8->V[X_REG(opcode)] = sum8;
}

void chip8_sub_reg(chip8 *c8, uint16_t opcode) {
	// Perform subtraction with two's complement
	uint16_t diff16 = c8->V[X_REG(opcode)] + (~c8->V[Y_REG(opcode)] & 0xFF) + 1;
	uint8_t diff8 = diff16 & 0xFF;

	c8->V[0xF] = !(diff16 & 0x100) ? 0 : 1; // VF reflects not borrow
	c8->V[X_REG(opcode)] = diff8;
}

void chip8_subn_reg(chip8 *c8, uint16_t opcode) {
	// Perform subtraction with two's complement
	uint16_t diff16 = c8->V[Y_REG(opcode)] + (~c8->V[X_REG(opcode)] & 0xFF) + 1;
	uint8_t diff8 = diff16 & 0xFF;

	c8->V[0xF] = !(diff16 & 0x100) ? 0 : 1; // VF reflects not borrow
	c8->V[X_REG(opcode)] = diff8;
}

void chip8_shr_reg(chip8 *c8, uint16_t opcode) {
	c8->V[0xF] = (c8->V[X_REG(opcode)] & 0x01) ? 1 : 0; // VF reflects LSB
	c8->V[X_REG(opcode)] >>= 1;
}

void chip8_shl_reg(chip8 *c8, uint16_t opcode) {
	c8->V[0xF] = (c8->V[X_REG(opcode)] & 0x80) ? 1 : 0; // VF reflects MSB
	c8->V[X_REG(opcode)] <<= 1;
}

void chip8_set_delay_timer(chip8 *c8, uint16_t opcode) {
	c8->delay_timer = c8->V[X_REG(opcode)];
}

void chip8_get_delay_timer(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] = c8->delay_timer;
}

void chip8_set_sound_timer(chip8 *c8, uint16_t opcode) {
	c8->sound_timer = c8->V[X_REG(opcode)];
}

void chip8_skip_key_down(chip8 *c8, uint16_t opcode) {
	if (c8->key[c8->V[X_REG(opcode)]]) {
		c8->PC += 2;
	}
}

void chip8_skip_key_up(chip8 *c8, uint16_t opcode) {
	if (!c8->key[c8->V[X_REG(opcode)]]) {
		c8->PC += 2;
	}
}

void chip8_wait_key(chip8 *c8, uint16_t opcode) {
	c8->PC -= 2; // Halt until next key press
	for (int i = 0; i < NUM_KEYS; i++) {
		if (c8->key[i]) {
			c8->V[X_REG(opcode)] = i;
			c8->PC += 2;
			break;
		}
	}
}

void chip8_randomize(chip8 *c8, uint16_t opcode) {
	c8->V[X_REG(opcode)] = (rand() % 0x100) & IMMEDIATE(opcode);
}

void chip8_draw_sprite(chip8 *c8, uint16_t opcode) {
	uint8_t x_coord = c8->V[X_REG(opcode)];
	uint8_t y_coord = c8->V[Y_REG(opcode)];

	uint8_t width = 8;
	uint8_t height = LOW_NIBBLE(opcode);

	c8->V[0xF] = 0;
	c8->draw_flag = true;

	for (int i = 0; i < height; i++) {
		uint8_t src_byte = c8->memory[c8->I + i];
		for (int j = 0; j < width; j++) {
			uint8_t bit_mask = 0x80 >> j;
			uint8_t src_bit = src_byte & bit_mask;

			// Source bit indicates the graphics pixel must flip
			if (src_bit) {
				uint16_t dest_idx = ((y_coord + i) * GFX_WIDTH + (x_coord + j)) % 0x800;

				// Record collision if destination pixel flips from set to unset after XOR operation
				if (c8->graphics[dest_idx]) {
					c8->V[0xF] = 1;
				}

				// Flip graphics pixel
				c8->graphics[dest_idx] ^= 0xFFFFFFFF;
			}
		}
	}
}

void chip8_load_addr(chip8 *c8, uint16_t opcode) {
	c8->I = ADDRESS(opcode);
}

void chip8_add_addr(chip8 *c8, uint16_t opcode) {
	c8->I += c8->V[X_REG(opcode)];
}

void chip8_load_font(chip8 *c8, uint16_t opcode) {
	c8->I = c8->V[X_REG(opcode)] * 5;
}

void chip8_store_BCD(chip8 *c8, uint16_t opcode) {
	uint8_t vx = c8->V[X_REG(opcode)];

	c8->memory[c8->I + 0] = (vx / 100)  % 10; // Hundreds digit
	c8->memory[c8->I + 1] = (vx / 10)   % 10; // Tens digit
	c8->memory[c8->I + 2] = (vx)        % 10; // Ones digit
}

void chip8_load_regs(chip8 *c8, uint16_t opcode) {
	for (int i = 0; i <= X_REG(opcode); i++) {
		c8->V[i] = c8->memory[c8->I + i];
	}
}

void chip8_store_regs(chip8 *c8, uint16_t opcode) {
	for (int i = 0; i <= X_REG(opcode); i++) {
		c8->memory[c8->I + i] = c8->V[i];
	}
}

void chip8_update_timers(chip8 *c8) {
	if (c8->sound_timer) {
		c8->sound_timer--;
	}

	if (c8->delay_timer) {
		c8->delay_timer--;
	}
}

void chip8_step(chip8 *c8, uint16_t opcode) {
	switch (HIGH_NIBBLE(opcode)) {
		case 0x0:
			switch (LOW_BYTE(opcode)) {
				case 0xE0:
					chip8_clear_screen(c8);
					break;
				case 0xEE:
					chip8_return(c8);
					break;
				default:
					fprintf(stderr, "Invalid opcode\n");
					exit(1);
			}
			break;
		case 0x1:
			chip8_jump(c8, opcode);
			break;
		case 0x2:
			chip8_call(c8, opcode);
			break;
		case 0x3:
			chip8_skip_eq_imm(c8, opcode);
			break;
		case 0x4:
			chip8_skip_neq_imm(c8, opcode);
			break;
		case 0x5:
			chip8_skip_eq_reg(c8, opcode);
			break;
		case 0x6:
			chip8_set_imm(c8, opcode);
			break;
		case 0x7:
			chip8_add_imm(c8, opcode);
			break;
		case 0x8:
			switch (LOW_NIBBLE(opcode)) {
				case 0x0:
					chip8_set_reg(c8, opcode);
					break;
				case 0x1:
					chip8_or_reg(c8, opcode);
					break;
				case 0x2:
					chip8_and_reg(c8, opcode);
					break;
				case 0x3:
					chip8_xor_reg(c8, opcode);
					break;
				case 0x4:
					chip8_add_reg(c8, opcode);
					break;
				case 0x5:
					chip8_sub_reg(c8, opcode);
					break;
				case 0x6:
					chip8_shr_reg(c8, opcode);
					break;
				case 0x7:
					chip8_subn_reg(c8, opcode);
					break;
				case 0xE:
					chip8_shl_reg(c8, opcode);
					break;
				default:
					fprintf(stderr, "Invalid opcode\n");
					exit(1);
			}
			break;
		case 0x9:
			chip8_skip_neq_reg(c8, opcode);
			break;
		case 0xA:
			chip8_load_addr(c8, opcode);
			break;
		case 0xB:
			chip8_jump_plus_r0(c8, opcode);
			break;
		case 0xC:
			chip8_randomize(c8, opcode);
			break;
		case 0xD:
			chip8_draw_sprite(c8, opcode);
			break;
		case 0xE:
			switch (LOW_BYTE(opcode)) {
				case 0x9E:
					chip8_skip_key_down(c8, opcode);
					break;
				case 0xA1:
					chip8_skip_key_up(c8, opcode);
					break;
				default:
					fprintf(stderr, "Invalid opcode\n");
					exit(1);
			}
			break;
		case 0xF:
			switch (LOW_BYTE(opcode)) {
				case 0x07:
					chip8_get_delay_timer(c8, opcode);
					break;
				case 0x0A:
					chip8_wait_key(c8, opcode);
					break;
				case 0x15:
					chip8_set_delay_timer(c8, opcode);
					break;
				case 0x18:
					chip8_set_sound_timer(c8, opcode);
					break;
				case 0x1E:
					chip8_add_addr(c8, opcode);
					break;
				case 0x29:
					chip8_load_font(c8, opcode);
					break;
				case 0x33:
					chip8_store_BCD(c8, opcode);
					break;
				case 0x55:
					chip8_store_regs(c8, opcode);
					break;
				case 0x65:
					chip8_load_regs(c8, opcode);
					break;
				default:
					fprintf(stderr, "Invalid opcode\n");
					exit(1);
			}
			break;
		default:
			fprintf(stderr, "Invalid opcode\n");
			exit(1);
	}

	chip8_update_timers(c8);
}
