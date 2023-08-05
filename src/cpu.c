
#include "../include/cpu.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "../include/register.h"

void cpu_init(Cpu* cpu) {
    for (int j = 0; j < CPU_REGISTERS; j++) {
        regSet32(&cpu->registers[j], 0);
    }
}

void cpu_set_zero_flag(Cpu* cpu, bool bit) {
    cpu->registers[R_FLAGS] = bit ? cpu->registers[R_FLAGS] | FLAG_ZERO
                                  : cpu->registers[R_FLAGS] & ~FLAG_ZERO;
}

bool cpu_get_zero_flag(Cpu* cpu) {
    return (cpu->registers[R_FLAGS] & FLAG_ZERO) == FLAG_ZERO;
}

void cpu_print_registers(Cpu* cpu, FILE* out) {
    fprintf(out, "Register\tHEX\t\t\tASCII\t\tDEC\t\t\t\tBIN\n");

    for (int j = 0; j < CPU_REGISTERS; j++) {
        uint32_t value = regGet32(&cpu->registers[j]);
        fprintf(out, "%s\t\t\t", register_names[j]);

        fprintf(out, "%08X\t", value);

        uint8_t* bytes = (void*)&value;

        for (int i = 0; i < 4; i++) {
            char ch = bytes[i];
            if (isprint(ch)) {
                fprintf(out, "%c", ch);
            } else {
                fprintf(out, ".");
            }
        }

        fprintf(out, "\t\t");

        fprintf(out, "%010u\t\t", value);

        for (int i = 0; i < 32; i++) {
            if (i % 8 == 0 && i != 0) {
                fprintf(out, " ");
            }
            fprintf(out, "%d", (value >> (31 - i)) & 1);
        }

        fprintf(out, "\n");
    }
}

// MOV

void cpu_mov_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg], *(uint8_t*)(imm_value));
            break;
        case 16:
            regSet16(&cpu->registers[reg], *(uint16_t*)(imm_value));
            break;
        case 32:
            regSet32(&cpu->registers[reg], *(uint32_t*)(imm_value));
            break;
    }
}

void cpu_mov_rr(Cpu* cpu, int size, int reg1, int reg2) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg1], regGet8(&cpu->registers[reg2]));
            break;
        case 16:
            regSet16(&cpu->registers[reg1], regGet16(&cpu->registers[reg2]));
            break;
        case 32:
            regSet32(&cpu->registers[reg1], regGet32(&cpu->registers[reg2]));
            break;
    }
}

// ADD

void cpu_add_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg],
                    regGet8(&cpu->registers[reg]) + *(uint8_t*)(imm_value));
            break;
        case 16:
            regSet16(&cpu->registers[reg],
                     regGet16(&cpu->registers[reg]) + *(uint16_t*)(imm_value));
            break;
        case 32:
            regSet32(&cpu->registers[reg],
                     regGet32(&cpu->registers[reg]) + *(uint32_t*)(imm_value));
            break;
    }
}

void cpu_add_rr(Cpu* cpu, int size, int reg1, int reg2) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg1], regGet8(&cpu->registers[reg1]) +
                                               regGet8(&cpu->registers[reg2]));
            break;
        case 16:
            regSet16(&cpu->registers[reg1],
                     regGet16(&cpu->registers[reg1]) +
                         regGet16(&cpu->registers[reg2]));
            break;
        case 32:
            regSet32(&cpu->registers[reg1],
                     regGet32(&cpu->registers[reg1]) +
                         regGet32(&cpu->registers[reg2]));
            break;
    }
}

// SUB

void cpu_sub_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg],
                    regGet8(&cpu->registers[reg]) - *(uint8_t*)(imm_value));
            break;
        case 16:
            regSet16(&cpu->registers[reg],
                     regGet16(&cpu->registers[reg]) - *(uint16_t*)(imm_value));
            break;
        case 32:
            regSet32(&cpu->registers[reg],
                     regGet32(&cpu->registers[reg]) - *(uint32_t*)(imm_value));
            break;
    }
}

void cpu_sub_rr(Cpu* cpu, int size, int reg1, int reg2) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg1], regGet8(&cpu->registers[reg1]) -
                                               regGet8(&cpu->registers[reg2]));
            break;
        case 16:
            regSet16(&cpu->registers[reg1],
                     regGet16(&cpu->registers[reg1]) -
                         regGet16(&cpu->registers[reg2]));
            break;
        case 32:
            regSet32(&cpu->registers[reg1],
                     regGet32(&cpu->registers[reg1]) -
                         regGet32(&cpu->registers[reg2]));
            break;
    }
}

// MUK

void cpu_mul_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg],
                    regGet8(&cpu->registers[reg]) * *(uint8_t*)(imm_value));
            break;
        case 16:
            regSet16(&cpu->registers[reg],
                     regGet16(&cpu->registers[reg]) * *(uint16_t*)(imm_value));
            break;
        case 32:
            regSet32(&cpu->registers[reg],
                     regGet32(&cpu->registers[reg]) * *(uint32_t*)(imm_value));
            break;
    }
}

void cpu_mul_rr(Cpu* cpu, int size, int reg1, int reg2) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg1], regGet8(&cpu->registers[reg1]) *
                                               regGet8(&cpu->registers[reg2]));
            break;
        case 16:
            regSet16(&cpu->registers[reg1],
                     regGet16(&cpu->registers[reg1]) *
                         regGet16(&cpu->registers[reg2]));
            break;
        case 32:
            regSet32(&cpu->registers[reg1],
                     regGet32(&cpu->registers[reg1]) *
                         regGet32(&cpu->registers[reg2]));
            break;
    }
}

// DIV

void cpu_div_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg],
                    regGet8(&cpu->registers[reg]) / *(uint8_t*)(imm_value));
            break;
        case 16:
            regSet16(&cpu->registers[reg],
                     regGet16(&cpu->registers[reg]) / *(uint16_t*)(imm_value));
            break;
        case 32:
            regSet32(&cpu->registers[reg],
                     regGet32(&cpu->registers[reg]) / *(uint32_t*)(imm_value));
            break;
    }
}

void cpu_div_rr(Cpu* cpu, int size, int reg1, int reg2) {
    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg1], regGet8(&cpu->registers[reg1]) /
                                               regGet8(&cpu->registers[reg2]));
            break;
        case 16:
            regSet16(&cpu->registers[reg1],
                     regGet16(&cpu->registers[reg1]) /
                         regGet16(&cpu->registers[reg2]));
            break;
        case 32:
            regSet32(&cpu->registers[reg1],
                     regGet32(&cpu->registers[reg1]) /
                         regGet32(&cpu->registers[reg2]));
            break;
    }
}

// CMP

void cpu_cmp_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    int acc = 0;

    switch (size) {
        case 8:
            acc = regGet8(&cpu->registers[reg]) - *(uint8_t*)(imm_value);
            break;
        case 16:
            acc = regGet16(&cpu->registers[reg]) - *(uint16_t*)(imm_value);
            break;
        case 32:
            acc = regGet32(&cpu->registers[reg]) - *(uint32_t*)(imm_value);
            break;
    }

    cpu_set_zero_flag(cpu, acc == 0);
}

void cpu_cmp_rr(Cpu* cpu, int size, int reg1, int reg2) {
    int acc = 0;

    switch (size) {
        case 8:
            acc =
                regGet8(&cpu->registers[reg1]) - regGet8(&cpu->registers[reg2]);
            break;
        case 16:
            acc = regGet16(&cpu->registers[reg1]) -
                  regGet16(&cpu->registers[reg2]);
            break;
        case 32:
            acc = regGet32(&cpu->registers[reg1]) -
                  regGet32(&cpu->registers[reg2]);
            break;
    }

    cpu_set_zero_flag(cpu, acc == 0);
}

// JMP

void cpu_jmp_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    cpu->registers[R_PROGRAM_COUNTER] =
        regGet32(&cpu->registers[reg]);
}

void cpu_jmp_i(Cpu* cpu, int size, uint8_t * imm_value) {
    assert(size == 32);

    cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
}

// JZ

void cpu_jz_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (cpu_get_zero_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jz_r(Cpu* cpu, int size, int reg1) {
    assert(size == 32);

    if (cpu_get_zero_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] =
            regGet32(&cpu->registers[reg1]);
    }
}

// JNZ

void cpu_jnz_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (!cpu_get_zero_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jnz_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    if (!cpu_get_zero_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] =
            regGet32(&cpu->registers[reg]);
    }
}

// LOAD

void cpu_load_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value, uint8_t* memory) {

    uint32_t addr = *(uint32_t*)(imm_value);

    switch (size) {
        case 8:
            regSet8(&cpu->registers[reg], *(uint8_t*)(&memory[addr]));
            break;
        case 16:
            regSet16(&cpu->registers[reg], *(uint16_t*)(&memory[addr]));
            break;
        case 32:
            regSet32(&cpu->registers[reg], *(uint32_t*)(&memory[addr]));
            break;
    }
}

void cpu_load_rr(Cpu* cpu, int size, int reg1, int reg2, uint8_t* memory) {
    
        uint32_t addr = regGet32(&cpu->registers[reg2]);
    
        switch (size) {
            case 8:
                regSet8(&cpu->registers[reg1], *(uint8_t*)(&memory[addr]));
                break;
            case 16:
                regSet16(&cpu->registers[reg1], *(uint16_t*)(&memory[addr]));
                break;
            case 32:
                regSet32(&cpu->registers[reg1], *(uint32_t*)(&memory[addr]));
                break;
        }
}

// STORE

void cpu_store_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value,
                  uint8_t* memory);

void cpu_store_rr(Cpu* cpu, int size, int reg1, int reg2, uint8_t* memory);

void cpu_store_ir(Cpu* cpu, int size, uint8_t* imm_value, int reg2,
                  uint8_t* memory);

// PUSH

void cpu_push_r(Cpu* cpu, int size, int reg, uint8_t* memory);

// POP

void cpu_pop_r(Cpu* cpu, int size, int reg, uint8_t* memory);

// SYSCALL

void cpu_syscall(Cpu* cpu, uint8_t* memory) {
    uint32_t r0 = regGet32(&cpu->registers[0]);
    uint32_t r1 = regGet32(&cpu->registers[1]);
    uint32_t r2 = regGet32(&cpu->registers[2]);
    uint32_t r3 = regGet32(&cpu->registers[3]);

    register uint64_t rdi asm("rdi") = r1;
    register char* rsi asm("rsi") = (char*)&memory[r2];
    register uint64_t rdx asm("rdx") = r3;
    register uint64_t rax asm("rax") = r0;

    asm("syscall");
}