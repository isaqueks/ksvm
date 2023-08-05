#ifndef CPU_H
#define CPU_H

#include <inttypes.h>
#include "register.h"
#include <stdio.h>

#define CPU_REGISTERS 32

#define R_STACK_POINTER 26 /* R26 */
#define R_BASE_POINTER 27  /* R27 */
#define R_PROGRAM_COUNTER 28 /* R28 */
#define R_FLAGS 29 /* R29 */

#define IS_GENERAL_REGISTER(reg) (reg >= 0 && reg <= 25)

typedef struct {
    Register_t registers[CPU_REGISTERS];
} Cpu;


void cpu_init(Cpu *cpu);

void cpu_print_registers(Cpu* cpu, FILE* out);


// MOV

void cpu_mov_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_mov_rr(Cpu* cpu, int size, int reg1, int reg2);

// ADD

void cpu_add_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_add_rr(Cpu* cpu, int size, int reg1, int reg2);

// SUB

void cpu_sub_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_sub_rr(Cpu* cpu, int size, int reg1, int reg2);

// MUK

void cpu_mul_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_mul_rr(Cpu* cpu, int size, int reg1, int reg2);

// DIV

void cpu_div_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_div_rr(Cpu* cpu, int size, int reg1, int reg2);

// CMP

void cpu_cmp_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value);

void cpu_cmp_rr(Cpu* cpu, int size, int reg1, int reg2);

// JMP

void cpu_jmp_r(Cpu* cpu, int size, int reg);

void cpu_jmp_i(Cpu* cpu, int size, uint8_t* imm_value);

// JZ

void cpu_jz_r(Cpu* cpu, int size, int reg);

void cpu_jz_i(Cpu* cpu, int size, uint8_t* imm_value);

// JNZ

void cpu_jnz_i(Cpu* cpu, int size, uint8_t* imm_value);

void cpu_jnz_r(Cpu* cpu, int size, int reg);

// LOAD

void cpu_load_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value, uint8_t* memory);

void cpu_load_rr(Cpu* cpu, int size, int reg1, int reg2, uint8_t* memory);


// STORE

void cpu_store_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value, uint8_t* memory);

void cpu_store_rr(Cpu* cpu, int size, int reg1, int reg2, uint8_t* memory);

void cpu_store_ir(Cpu* cpu, int size, uint8_t* imm_value, int reg2, uint8_t* memory);

// PUSH

void cpu_push_r(Cpu* cpu, int size, int reg, uint8_t* memory);


// POP

void cpu_pop_r(Cpu* cpu, int size, int reg, uint8_t* memory);

// SYSCALL

void cpu_syscall(Cpu* cpu, uint8_t* memory);


#endif