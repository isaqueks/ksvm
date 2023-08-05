#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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


void vm_init(VM* vm, void* mem, uint32_t memory_size) {
    cpu_init(&vm->cpu);
    vm->memory = mem;
    vm->memory_size = memory_size;
}

void vm_load_program(VM* vm, uint8_t* program, uint32_t program_size) {

    int program_addr = 0;
    SET_PC(vm, program_addr);

    memcpy(&vm->memory[program_addr], program, program_size);

    regSet32(&vm->cpu.registers[R_STACK_POINTER], vm->memory_size - 1);
    regSet32(&vm->cpu.registers[R_BASE_POINTER], vm->memory_size - 1);
}

void vm_print_instruction_table(FILE* out, char* instr, char* op1, char* op2) {
    if (op1 == NULL) {
        fprintf(out, "%s\n", instr);
    } else if (op2 == NULL) {
        fprintf(out, "%s\t\t%s\n", instr, op1);
    } else {
        fprintf(out, "%s\t\t%s\t\t%s\n", instr, op1, op2);
    }
}

void vm_print_instruction_rr(FILE* out, char* instr, uint16_t r1, uint16_t r2) {
    vm_print_instruction_table(out, instr, register_names[r1], register_names[r2]);
}

void vm_print_instruction_ri(FILE* out, char* instr, uint16_t r1, void* op2, int size) {
    char buffer[32] = { 0 };
    switch (size) {
        case 8:
            sprintf(buffer, "%02X", *CAST(uint8_t*, op2));
            break;
        case 16:
            sprintf(buffer, "%04X", *CAST(uint16_t*, op2));
            break;
        case 32:
            sprintf(buffer, "%08X", *CAST(uint32_t*, op2));
            break;
    }
    vm_print_instruction_table(out, instr, register_names[r1], buffer);
}

void vm_print_instruction_r(FILE* out, char* instr, uint16_t r1) {
    vm_print_instruction_table(out, instr, register_names[r1], NULL);
}

void vm_print_instruction_i(FILE* out, char* instr, void* op1, int size) {
    char buffer[32] = { 0 };
    switch (size) {
        case 8:
            sprintf(buffer, "%02X", *CAST(uint8_t*, op1));
            break;
        case 16:
            sprintf(buffer, "%04X", *CAST(uint16_t*, op1));
            break;
        case 32:
            sprintf(buffer, "%08X", *CAST(uint32_t*, op1));
            break;
    }
    vm_print_instruction_table(out, instr, buffer, NULL);
}

void vm_print_instruction(VM* vm, InstructionOpcode opcode, uint8_t* operands, FILE* out) {

    InstructionDef* def = instruction_get_def(opcode);

    switch (opcode) {
        case OP_MOV8RI:
            vm_print_instruction_ri(out, "MOV8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_MOV16RI:
            vm_print_instruction_ri(out, "MOV16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_MOV32RI:
            vm_print_instruction_ri(out, "MOV32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_MOV8RR:
            vm_print_instruction_rr(out, "MOV8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV16RR:
            vm_print_instruction_rr(out, "MOV16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MOV32RR:
            vm_print_instruction_rr(out, "MOV32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_ADD8RI:
            vm_print_instruction_ri(out, "ADD8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_ADD16RI:
            vm_print_instruction_ri(out, "ADD16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_ADD32RI:
            vm_print_instruction_ri(out, "ADD32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_ADD8RR:
            vm_print_instruction_rr(out, "ADD8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD16RR:
            vm_print_instruction_rr(out, "ADD16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_ADD32RR:
            vm_print_instruction_rr(out, "ADD32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_SUB8RI:
            vm_print_instruction_ri(out, "SUB8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_SUB16RI:
            vm_print_instruction_ri(out, "SUB16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_SUB32RI:
            vm_print_instruction_ri(out, "SUB32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_SUB8RR:
            vm_print_instruction_rr(out, "SUB8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB16RR:
            vm_print_instruction_rr(out, "SUB16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_SUB32RR:
            vm_print_instruction_rr(out, "SUB32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_MUL8RI:
            vm_print_instruction_ri(out, "MUL8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_MUL16RI:
            vm_print_instruction_ri(out, "MUL16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_MUL32RI:
            vm_print_instruction_ri(out, "MUL32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_MUL8RR:
            vm_print_instruction_rr(out, "MUL8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MUL16RR:
            vm_print_instruction_rr(out, "MUL16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_MUL32RR:
            vm_print_instruction_rr(out, "MUL32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_DIV8RI:
            vm_print_instruction_ri(out, "DIV8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_DIV16RI:
            vm_print_instruction_ri(out, "DIV16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_DIV32RI:
            vm_print_instruction_ri(out, "DIV32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_DIV8RR:
            vm_print_instruction_rr(out, "DIV8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_DIV16RR:
            vm_print_instruction_rr(out, "DIV16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_DIV32RR:
            vm_print_instruction_rr(out, "DIV32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_CMP8RI:
            vm_print_instruction_ri(out, "CMP8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_CMP16RI:
            vm_print_instruction_ri(out, "CMP16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_CMP32RI:
            vm_print_instruction_ri(out, "CMP32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_CMP8RR:
            vm_print_instruction_rr(out, "CMP8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP16RR:
            vm_print_instruction_rr(out, "CMP16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_CMP32RR:
            vm_print_instruction_rr(out, "CMP32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_JMP32R:
            vm_print_instruction_r(out, "JMP32", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JMP32I:
            vm_print_instruction_i(out, "JMP32",  &operands[0], 32);
            break;

        case OP_JZ32R:
            vm_print_instruction_r(out, "JZ32", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JZ32I:
            vm_print_instruction_i(out, "JZ32", &operands[0], 32);
            break;

        case OP_JNZ32R:
            vm_print_instruction_r(out, "JNZ32", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_JNZ32I:
            vm_print_instruction_i(out, "JNZ32", &operands[0], 32);
            break;

        case OP_LOAD8RI:
            vm_print_instruction_ri(out, "LOAD8", *CAST(uint16_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_LOAD16RI:
            vm_print_instruction_ri(out, "LOAD16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_LOAD32RI:
            vm_print_instruction_ri(out, "LOAD32", *CAST(uint16_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_LOAD8RR:
            vm_print_instruction_rr(out, "LOAD8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_LOAD16RR:
            vm_print_instruction_rr(out, "LOAD16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_LOAD32RR:
            vm_print_instruction_rr(out, "LOAD32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_STORE8RI:
            vm_print_instruction_ri(out, "STORE8", *CAST(uint8_t*, &operands[0]), &operands[2], 8);
            break;
        case OP_STORE16RI:
            vm_print_instruction_ri(out, "STORE16", *CAST(uint16_t*, &operands[0]), &operands[2], 16);
            break;
        case OP_STORE32RI:
            vm_print_instruction_ri(out, "STORE32", *CAST(uint32_t*, &operands[0]), &operands[2], 32);
            break;

        case OP_STORE8RR:
            vm_print_instruction_rr(out, "STORE8", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_STORE16RR:
            vm_print_instruction_rr(out, "STORE16", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;
        case OP_STORE32RR:
            vm_print_instruction_rr(out, "STORE32", *CAST(uint16_t*, &operands[0]), *CAST(uint16_t*, &operands[2]));
            break;

        case OP_PUSH8I:
            vm_print_instruction_i(out, "PUSH8", &operands[0], 8);
            break;
        case OP_PUSH16I:
            vm_print_instruction_i(out, "PUSH16", &operands[0], 16);
            break;
        case OP_PUSH32I:
            vm_print_instruction_i(out, "PUSH32", &operands[0], 32);
            break;

        case OP_PUSH8R:
            vm_print_instruction_r(out, "PUSH8", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_PUSH16R:
            vm_print_instruction_r(out, "PUSH16", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_PUSH32R:
            vm_print_instruction_r(out, "PUSH32", *CAST(uint16_t*, &operands[0]));
            break;

        case OP_POP8R:
            vm_print_instruction_r(out, "POP8", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_POP16R:
            vm_print_instruction_r(out, "POP16", *CAST(uint16_t*, &operands[0]));
            break;
        case OP_POP32R:
            vm_print_instruction_r(out, "POP32", *CAST(uint16_t*, &operands[0]));
            break;

        case OP_SYSCALL:
            fprintf(out, "SYSCALL\n");
            break;

        default:
            fprintf(out, "Unknown instruction: %04X (PC: %d)\n", opcode, regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]));
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
            cpu_jmp_i(&vm->cpu, 32, &operands[0]);
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

void vm_dump_program(VM* vm, char* program, int size, FILE* out) {

    int pc = 0;

    fprintf(out, "PC     00 01 02 03 04 05 06 07 \t\tINST\t\tOP1\t\tOP2\t\t\tASCII\n\n");

    while (pc < size) {
        uint8_t opcode[2] = {0, 0};
        memcpy((void*)opcode, &program[pc], 2);

        InstructionDef* def = instruction_get_def(*CAST(uint16_t*, opcode));

        int size = 2;

        if (def != NULL) {
            size = instruction_get_size(def);
        }
        else {
            for (size = 2; size < 8; size += 2) {
                if (instruction_get_def(*CAST(uint16_t*, &program[pc + size])) != NULL) {
                    break;
                }
            }
        }

        char bytes[8] = { 0x00 };
        memcpy(bytes, &program[pc], size);

        fprintf(out, "%04X   ", pc);

        for (int i = 0; i < size; i++) {
            fprintf(out, "%02X ", bytes[i]);
        }

        for (int i = size; i < 7; i++) {
            fprintf(out, "   ");
        }

        fprintf(out, "\t\t");

        if (def == NULL) {
            fprintf(out, "????\t\t\t\t\t\t\t");
            // fprintf(out, "[DEBUG] Unknown opcode: %04X (INDEX: %08X)\n", opcode, pc);
            for (int i = 0; i < size; i++) {
                char ch = bytes[i];
                if (isprint(ch)) {
                    fprintf(out, "%c", ch);
                }
                else {
                    fprintf(out, ".");
                }
            }
            fprintf(out, "\n");
        }
        else {
            vm_print_instruction(vm, *CAST(uint16_t*, opcode), &program[pc + 2], out);
        }


        pc += size;
    }
}

void vm_run(VM* vm, FILE* debugger_out) {
    while (!FLAG(FLAG_ERROR) && regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]) < vm->memory_size) {

        uint32_t pc = regGet32(&vm->cpu.registers[R_PROGRAM_COUNTER]);
        uint8_t opcode[] = {vm->memory[pc], vm->memory[pc+1]};

        if (debugger_out != NULL) {
            vm_print_instruction(vm, *CAST(uint16_t*, opcode), &vm->memory[pc + 2], debugger_out);
            fflush(debugger_out);
        }
        vm_compute_instruction(vm, *CAST(uint16_t*, opcode), &vm->memory[pc + 2]);
    }
}