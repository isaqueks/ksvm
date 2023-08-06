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


char* register_names[] = {
    "EAX",
    "EBX",
    "ECX",
    "EDX",
    "R4",
    "R5",
    "R6",
    "R7",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15",

    "R16",
    "R17",
    "R18",
    "R19",
    "R20",
    "R21",
    "R22",
    "R23",
    "R24",
    "R25",

    "ESP",
    "EBP",
    "EIP",
    "FLAGS",
    "R30",
    "R31"
};