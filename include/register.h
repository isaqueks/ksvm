#ifndef REGISTERS_H
#define REGISTERS_H

#include <inttypes.h>

typedef uint32_t Register_t;

void regSet32(Register_t *reg, uint32_t value);
void regSet16(Register_t *reg, uint16_t value);
void regSet8(Register_t *reg, uint8_t value);

uint32_t regGet32(Register_t *reg);
uint16_t regGet16(Register_t *reg);
uint8_t regGet8(Register_t *reg);

#define REG_LIT(REG) REG, 0x00

#define FLAG_ZERO       0b00000000000000000000000000000001
#define FLAG_ERROR      0b00000000000000000000000000000010


#endif