#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/vm.h"

uint8_t vm_read_memory(VM* vm, uint32_t address) {
    return vm->memory[address];
}

void vm_write_memory(VM* vm, uint32_t address, uint8_t value) {
    vm->memory[address] = value;
}


#define CAST(T, ptr) ((T)(void*)ptr)
#define SET_PC(vm, value) regSet32(&vm->cpu.registers[R_PROGRAM_COUNTER], value)
#define PC_INC(i)  SET_PC(vm, regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]) + i)
#define FLAG(mask) (regGet32(&vm->cpu.registers[R_FLAGS]) & mask)
#define ENABLE_FLAG(flag) regSet32(&vm->cpu.registers[R_FLAGS], regGet32(&vm->cpu.registers[R_FLAGS]) | flag)
#define DISABLE_FLAG(flag) regSet32(&vm->cpu.registers[R_FLAGS], regGet32(&vm->cpu.registers[R_FLAGS]) & ~flag)


void vm_init(VM* vm, uint32_t memory_size) {
    cpu_init(&vm->cpu);
    vm->memory = malloc(memory_size);
    vm->memory_size = memory_size;
}

void vm_load_program(VM* vm, uint8_t* program, uint32_t program_size) {

    int program_addr = 0;
    SET_PC(vm, program_addr);

    memcpy(&vm->memory[program_addr], program, program_size);
}

void vm_print_instruction(VM* vm, InstructionOpcode opcode, uint8_t* operands, FILE* out) {

    InstructionDef* def = instruction_get_def(opcode);

    switch (opcode) {
        case OP_MOV8RI:
            fprintf(out, "MOV8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_MOV16RI:
            fprintf(out, "MOV16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV32RI:
            fprintf(out, "MOV32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_MOV8RR:
            fprintf(out, "MOV8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV16RR:
            fprintf(out, "MOV16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV32RR:
            fprintf(out, "MOV32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_ADD8RI:
            fprintf(out, "ADD8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_ADD16RI:
            fprintf(out, "ADD16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD32RI:
            fprintf(out, "ADD32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_ADD8RR:
            fprintf(out, "ADD8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD16RR:
            fprintf(out, "ADD16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD32RR:
            fprintf(out, "ADD32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_SUB8RI:
            fprintf(out, "SUB8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_SUB16RI:
            fprintf(out, "SUB16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB32RI:
            fprintf(out, "SUB32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_SUB8RR:
            fprintf(out, "SUB8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB16RR:
            fprintf(out, "SUB16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB32RR:
            fprintf(out, "SUB32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_MUL8RI:
            fprintf(out, "MUL8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_MUL16RI:
            fprintf(out, "MUL16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MUL32RI:
            fprintf(out, "MUL32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_MUL8RR:
            fprintf(out, "MUL8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MUL16RR:
            fprintf(out, "MUL16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MUL32RR:
            fprintf(out, "MUL32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_DIV8RI:
            fprintf(out, "DIV8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_DIV16RI:
            fprintf(out, "DIV16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_DIV32RI:
            fprintf(out, "DIV32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_DIV8RR:
            fprintf(out, "DIV8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_DIV16RR:
            fprintf(out, "DIV16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_DIV32RR:
            fprintf(out, "DIV32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_CMP8RI:
            fprintf(out, "CMP8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_CMP16RI:
            fprintf(out, "CMP16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP32RI:
            fprintf(out, "CMP32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_CMP8RR:
            fprintf(out, "CMP8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP16RR:
            fprintf(out, "CMP16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP32RR:
            fprintf(out, "CMP32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_JMP32R:
            fprintf(out, "JMP32\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JMP32I:
            fprintf(out, "JMP32\t%08X\n", *CAST(uint32_t*, &operands[0]));
            break;

        case OP_JZ32R:
            fprintf(out, "JZ32\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JZ32I:
            fprintf(out, "JZ32\t%08X\n", *CAST(uint32_t*, &operands[0]));
            break;

        case OP_JNZ32R:
            fprintf(out, "JNZ32\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JNZ32I:
            fprintf(out, "JNZ32\t%08X\n", *CAST(uint32_t*, &operands[0]));
            break;

        case OP_LOAD8RI:
            fprintf(out, "LOAD8\tR%d\t%02X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint8_t*, &operands[2]));
            break;
        case OP_LOAD16RI:
            fprintf(out, "LOAD16\tR%d\t%04X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_LOAD32RI:
            fprintf(out, "LOAD32\tR%d\t%08X\n", *CAST(uint16_t*, &operands[0]), *CAST(uint32_t*, &operands[2]));
            break;

        case OP_LOAD8RR:
            fprintf(out, "LOAD8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_LOAD16RR:
            fprintf(out, "LOAD16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_LOAD32RR:
            fprintf(out, "LOAD32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_STORE8RI:
            fprintf(out, "STORE8\t%02X\tR%d\n", *CAST(uint8_t*, &operands[0]), *CAST(uint16_t*, &operands[1]));
            break;
        case OP_STORE16RI:
            fprintf(out, "STORE16\t%04X\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_STORE32RI:
            fprintf(out, "STORE32\t%08X\tR%d\n", *CAST(uint32_t*, &operands[0]), *CAST(uint16_t*, &operands[4]));
            break;

        case OP_STORE8RR:
            fprintf(out, "STORE8\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_STORE16RR:
            fprintf(out, "STORE16\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_STORE32RR:
            fprintf(out, "STORE32\tR%d\tR%d\n", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_PUSH8I:
            fprintf(out, "PUSH8\t%02X\n", *CAST(uint8_t*, &operands[0]));
            break;
        case OP_PUSH16I:
            fprintf(out, "PUSH16\t%04X\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_PUSH32I:
            fprintf(out, "PUSH32\t%08X\n", *CAST(uint32_t*, &operands[0]));
            break;

        case OP_PUSH8R:
            fprintf(out, "PUSH8\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_PUSH16R:
            fprintf(out, "PUSH16\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_PUSH32R:
            fprintf(out, "PUSH32\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;

        case OP_POP8R:
            fprintf(out, "POP8\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_POP16R:
            fprintf(out, "POP16\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_POP32R:
            fprintf(out, "POP32\tR%d\n", *CAST(uint16_t*, &operands[0]));
            break;

        case OP_SYSCALL:
            fprintf(out, "SYSCALL\n");
            break;

        default:
            fprintf(out, "Unknown instruction: %04X (INDEX: %d)\n", opcode, regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]));
    }
}


void vm_compute_instruction(VM* vm, InstructionOpcode opcode, uint8_t* operands) {

    InstructionDef* def = instruction_get_def(opcode);
    if (def == NULL) {
        fprintf(stderr, "Unknown opcode: %04X (INDEX: %d)\n", opcode, regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]));
        ENABLE_FLAG(FLAG_ERROR);
        return;
    }

    PC_INC(instruction_get_size(def));

    switch (opcode) {

        case OP_MOV8RI:
            cpu_mov_ri(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_MOV16RI:
            cpu_mov_ri(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_MOV32RI:
            cpu_mov_ri(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;

        case OP_MOV8RR:
            cpu_mov_rr(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV16RR:
            cpu_mov_rr(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV32RR:
            cpu_mov_rr(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_ADD8RI:
            cpu_add_ri(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_ADD16RI:
            cpu_add_ri(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_ADD32RI:
            cpu_add_ri(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;

        case OP_ADD8RR:
            cpu_add_rr(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD16RR:
            cpu_add_rr(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD32RR:
            cpu_add_rr(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_SUB8RR:
            cpu_sub_rr(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB16RR:
            cpu_sub_rr(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB32RR:
            cpu_sub_rr(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        
        case OP_CMP8RR:
            cpu_cmp_rr(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP16RR:
            cpu_cmp_rr(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP32RR:
            cpu_cmp_rr(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_CMP8RI:
            cpu_cmp_ri(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_CMP16RI:
            cpu_cmp_ri(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;
        case OP_CMP32RI:
            cpu_cmp_ri(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), &operands[2]);
            break;

        case OP_JMP32R:
            cpu_jmp_r(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]));
            break;

        case OP_JMP32I:
            cpu_jmp_r(&vm->cpu, 32, operands[0]);
            break;


        case OP_JZ32R:
            cpu_jz_r(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]));
            break;

        case OP_JZ32I:
            cpu_jz_i(&vm->cpu, 32, &operands[0]);
            break;

        case OP_JNZ32R:
            cpu_jnz_r(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]));
            break;

        case OP_JNZ32I:
            cpu_jnz_i(&vm->cpu, 32, &operands[0]);
            break;
    
        case OP_LOAD8RR:
            cpu_load_rr(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]), vm->memory);
            break;
        case OP_LOAD16RR:
            cpu_load_rr(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]), vm->memory);
            break;
        case OP_LOAD32RR:
            cpu_load_rr(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]), vm->memory);
            break;

        case OP_LOAD8RI:
            cpu_load_ri(&vm->cpu, 8, *CAST(uint16_t*, &operands[0]), &operands[2], vm->memory);
            break;
        case OP_LOAD16RI:
            cpu_load_ri(&vm->cpu, 16, *CAST(uint16_t*, &operands[0]), &operands[2], vm->memory);
            break;
        case OP_LOAD32RI:
            cpu_load_ri(&vm->cpu, 32, *CAST(uint16_t*, &operands[0]), &operands[2], vm->memory);
            break;


        case OP_SYSCALL:
            cpu_syscall(&vm->cpu, vm->memory);
            break;

    }

}

void vm_run(VM* vm, FILE* debugger_out) {
    while (!FLAG(FLAG_ERROR) && regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]) < vm->memory_size) {
        uint32_t pc = regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]);
        uint8_t opcode[] = {0x00, 0x00};
        memcpy((void*)opcode, &vm->memory[pc], 2);
        if (debugger_out != NULL) {
            vm_print_instruction(vm, *CAST(uint16_t*, opcode), &vm->memory[pc + 2], debugger_out);
        }
        vm_compute_instruction(vm, *CAST(uint16_t*, opcode), &vm->memory[pc + 2]);
    }
}