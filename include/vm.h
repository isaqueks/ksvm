#ifndef VM_H
#define VM_H

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include "instruction.h"
#include "register.h"
#include "cpu.h"

typedef struct {

    Cpu cpu;
    uint8_t* memory;
    uint32_t memory_size;

} VM;

typedef void (*vm_step_callback_t)(VM* vm, int pc);

void vm_init(VM* vm, void* mem, uint32_t memory_size);

void vm_load_program(VM* vm, uint8_t* program, uint32_t program_size);

void vm_compute_instruction(VM* vm, InstructionOpcode opcode, uint8_t* operands);

void vm_dump_program(VM* vm, char* program, int start, int size, int highlight_pc, FILE* out);

void vm_print_instruction(VM* vm, InstructionOpcode opcode, uint8_t* operands, FILE* out);

int vm_run(VM* vm, FILE* backlog, vm_step_callback_t on_step);

#endif