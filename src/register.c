#include "../include/register.h"

// Little endian

void regSet32(Register_t *reg, uint32_t value) {
    *reg = value;
}

void regSet16(Register_t *reg, uint16_t value) {
    uint16_t *reg16 = (void *) reg;
    reg16[0] = value;
}

void regSet8(Register_t *reg, uint8_t value) {
    uint8_t *reg8 = (void *) reg;
    reg8[0] = value;
}

uint32_t regGet32(Register_t *reg) {
    return *reg;
}

uint16_t regGet16(Register_t *reg) {
    uint16_t *reg16 = (void *) reg;
    return reg16[0];
}

uint8_t regGet8(Register_t *reg) {
    uint8_t *reg8 = (void *) reg;
    return reg8[0];
}
