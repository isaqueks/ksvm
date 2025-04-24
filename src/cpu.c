
#include "../include/cpu.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <math.h>

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

void cpu_set_carry_flag(Cpu* cpu, bool bit) {
    cpu->registers[R_FLAGS] = bit ? cpu->registers[R_FLAGS] | FLAG_CARRY
                                  : cpu->registers[R_FLAGS] & ~FLAG_CARRY;
}

bool cpu_get_carry_flag(Cpu* cpu) {
    return (cpu->registers[R_FLAGS] & FLAG_CARRY) == FLAG_CARRY;
}

void cpu_print_registers(Cpu* cpu, FILE* out) {
    fprintf(out, "Register\tHEX\t\tASCII\t\tDEC\t\t\tBIN\n");

    for (int j = 0; j < CPU_REGISTERS; j++) {
        uint32_t value = regGet32(&cpu->registers[j]);
        fprintf(out, "%s\t\t", register_names[j]);

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
    uint64_t sum = 0;
    switch (size) {
        case 8:
            sum = regGet8(&cpu->registers[reg]) + *(uint8_t*)(imm_value);
            regSet8(&cpu->registers[reg], (uint8_t)sum);
            break;
        case 16:
            sum = regGet16(&cpu->registers[reg]) + *(uint16_t*)(imm_value);
            regSet16(&cpu->registers[reg], (uint16_t)sum);
            break;
        case 32:
            sum = regGet32(&cpu->registers[reg]) + *(uint32_t*)(imm_value);
            regSet32(&cpu->registers[reg], (uint32_t)sum);
            break;
    }

    uint64_t max = (uint64_t)pow(2, size) - 1;
    cpu_set_carry_flag(cpu, sum > max);
    cpu_set_zero_flag(cpu, sum == 0);
}

void cpu_add_rr(Cpu* cpu, int size, int reg1, int reg2) {
    uint64_t sum = 0;
    switch (size) {
        case 8:
            sum = regGet8(&cpu->registers[reg1]) + regGet8(&cpu->registers[reg2]);
            regSet8(&cpu->registers[reg1], (uint8_t)sum);
            break;
        case 16:
            sum = regGet16(&cpu->registers[reg1]) + regGet16(&cpu->registers[reg2]);
            regSet16(&cpu->registers[reg1], (uint16_t)sum);
            break;
        case 32:
            sum = regGet32(&cpu->registers[reg1]) + regGet32(&cpu->registers[reg2]);
            regSet32(&cpu->registers[reg1], (uint32_t)sum);
            break;
    }

    uint64_t max = (uint64_t)pow(2, size) - 1;
    cpu_set_carry_flag(cpu, sum > max);
    cpu_set_zero_flag(cpu, sum == 0);
}

// SUB

void cpu_sub_ri(Cpu* cpu, int size, int reg, uint8_t* imm_value) {
    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t sub = 0;

    switch (size) {
        case 8:
            a = regGet8(&cpu->registers[reg]);
            b = *(uint8_t*)(imm_value);
            sub = a - b;
            regSet8(&cpu->registers[reg], (uint8_t)sub);
            break;
        case 16:
            a = regGet16(&cpu->registers[reg]);
            b = *(uint16_t*)(imm_value);
            sub = a - b;
            regSet16(&cpu->registers[reg], sub);
            break;
        case 32:
            a = regGet32(&cpu->registers[reg]);
            b = *(uint32_t*)(imm_value);
            sub = a - b;
            regSet32(&cpu->registers[reg], sub);
            break;
    }

    cpu_set_carry_flag(cpu, sub > a);    
    cpu_set_zero_flag(cpu, sub == 0);
}

void cpu_sub_rr(Cpu* cpu, int size, int reg1, int reg2) {
    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t sub = 0;

    switch (size) {
        case 8:
            a = regGet8(&cpu->registers[reg1]);
            b = regGet8(&cpu->registers[reg2]);
            sub = a - b;

            regSet8(&cpu->registers[reg1], sub);
            break;
        case 16:
            a = regGet16(&cpu->registers[reg1]);
            b = regGet16(&cpu->registers[reg2]);
            sub = a - b;

            regSet16(&cpu->registers[reg1], sub);
            break;
        case 32:
            a = regGet32(&cpu->registers[reg1]);
            b = regGet32(&cpu->registers[reg2]);
            sub = a - b;

            regSet32(&cpu->registers[reg1], sub);
            break;
    }

    cpu_set_carry_flag(cpu, sub > a);
    cpu_set_zero_flag(cpu, sub == 0);
}

// MUL

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
    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t sub = 0;

    switch (size) {
        case 8:
            a = regGet8(&cpu->registers[reg]);
            b = *(uint8_t*)(imm_value);
            sub = a - b;

            break;
        case 16:
            a = regGet16(&cpu->registers[reg]);
            b = *(uint16_t*)(imm_value);
            sub = a - b;

            break;
        case 32:
            a = regGet32(&cpu->registers[reg]);
            b = *(uint32_t*)(imm_value);
            sub = a - b;

            break;
    }

    cpu_set_carry_flag(cpu, sub > a);    
    cpu_set_zero_flag(cpu, sub == 0);
}

void cpu_cmp_rr(Cpu* cpu, int size, int reg1, int reg2) {
    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t sub = 0;

    switch (size) {
        case 8:
            a = regGet8(&cpu->registers[reg1]);
            b = regGet8(&cpu->registers[reg2]);
            sub = a - b;

            break;
        case 16:
            a = regGet16(&cpu->registers[reg1]);
            b = regGet16(&cpu->registers[reg2]);
            sub = a - b;

            break;
        case 32:
            a = regGet32(&cpu->registers[reg1]);
            b = regGet32(&cpu->registers[reg2]);
            sub = a - b;

            break;
    }

    cpu_set_carry_flag(cpu, sub > a);
    cpu_set_zero_flag(cpu, sub == 0);
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

// JG

void cpu_jg_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (!cpu_get_zero_flag(cpu) && !cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jg_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    if (!cpu_get_zero_flag(cpu) && !cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] =
            regGet32(&cpu->registers[reg]);
    }
}

// JGE

void cpu_jge_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (!cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jge_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    if (!cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] =
            regGet32(&cpu->registers[reg]);
    }
}

// JL

void cpu_jl_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jl_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    if (cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] =
            regGet32(&cpu->registers[reg]);
    }
}

// JLE

void cpu_jle_i(Cpu* cpu, int size, uint8_t* imm_value) {
    assert(size == 32);

    if (cpu_get_zero_flag(cpu) || cpu_get_carry_flag(cpu)) {
        cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(imm_value);
    }
}

void cpu_jle_r(Cpu* cpu, int size, int reg) {
    assert(size == 32);

    if (cpu_get_zero_flag(cpu) || cpu_get_carry_flag(cpu)) {
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

void cpu_push_r(Cpu* cpu, int size, int reg, uint8_t* memory) {
    switch (size) {
        case 32:
            cpu->registers[R_STACK_POINTER] -= 4;
            *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                regGet32(&cpu->registers[reg]);
            break;
        case 16:
            cpu->registers[R_STACK_POINTER] -= 2;
            *(uint16_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                regGet16(&cpu->registers[reg]);
            break;
        case 8:
            cpu->registers[R_STACK_POINTER] -= 1;
            *(uint8_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                regGet8(&cpu->registers[reg]);
            break;
    }
}

void cpu_push_i(Cpu* cpu, int size, uint8_t* imm_value, uint8_t* memory) {

    switch (size) {
        case 32:
            cpu->registers[R_STACK_POINTER] -= 4;
            *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                *(uint32_t*)(imm_value);
            break;
        case 16:
            cpu->registers[R_STACK_POINTER] -= 2;
            *(uint16_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                *(uint16_t*)(imm_value);
            break;
        case 8:
            cpu->registers[R_STACK_POINTER] -= 1;
            *(uint8_t*)(&memory[cpu->registers[R_STACK_POINTER]]) =
                *(uint8_t*)(imm_value);
            break;
    }

}

// POP

void cpu_pop_r(Cpu* cpu, int size, int reg, uint8_t* memory) {
    switch (size) {
        case 32:
            regSet32(&cpu->registers[reg],
                     *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]));
            cpu->registers[R_STACK_POINTER] += 4;
            break;
        case 16:
            regSet16(&cpu->registers[reg],
                     *(uint16_t*)(&memory[cpu->registers[R_STACK_POINTER]]));
            cpu->registers[R_STACK_POINTER] += 2;
            break;
        case 8:
            regSet8(&cpu->registers[reg],
                    *(uint8_t*)(&memory[cpu->registers[R_STACK_POINTER]]));
            cpu->registers[R_STACK_POINTER] += 1;
            break;
    }
}

// CALL

void cpu_call_r(Cpu* cpu, int size, int reg, uint8_t* memory) {
    assert(size == 32);

    cpu->registers[R_STACK_POINTER] -= 4;
    *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]) = cpu->registers[R_PROGRAM_COUNTER]/* + 4*/;

    cpu->registers[R_PROGRAM_COUNTER] = regGet32(&cpu->registers[reg]);
}

void cpu_call_i(Cpu* cpu, int size, uint8_t* imm_value, uint8_t* memory) {
    assert(size == 32);

    cpu->registers[R_STACK_POINTER] -= 4;
    *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]) = cpu->registers[R_PROGRAM_COUNTER]/* + 6*/;

    cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)imm_value;
}

// RET

void cpu_ret(Cpu* cpu, uint8_t* memory) {
    cpu->registers[R_PROGRAM_COUNTER] = *(uint32_t*)(&memory[cpu->registers[R_STACK_POINTER]]);
    cpu->registers[R_STACK_POINTER] += 4;
}

// SYSCALL

uint8_t syscall_mem_ptrs[] = {
    0b010, /* Read - RDI, RSI (ptr), RDX */
    0b010, /* Write - RDI, RSI (ptr), RDX */
    0b100, /* Open - RDI (ptr), RSI, RDX */
    0b000, /* Close - RDI */
};

void cpu_syscall(Cpu* cpu, uint8_t* memory) {
    
    uint64_t r0 = (uint64_t)regGet32(&cpu->registers[0]);
    uint64_t r1 = (uint64_t)regGet32(&cpu->registers[1]);
    uint64_t r2 = (uint64_t)regGet32(&cpu->registers[2]);
    uint64_t r3 = (uint64_t)regGet32(&cpu->registers[3]);

    if (r0 < sizeof(syscall_mem_ptrs)) {
        uint8_t opts = syscall_mem_ptrs[r0];
        if (opts & 0b100) {
            r1 += (uint64_t)memory;
        }
        if (opts & 0b010) {
            r2 += (uint64_t)memory;
        }
        if (opts & 0b001) {
            r3 += (uint64_t)memory;
        }
    }

    register uint64_t rdi asm("rdi") = r1;
    register uint64_t rsi asm("rsi") = r2;
    register uint64_t rdx asm("rdx") = r3;
    register uint64_t rax asm("rax") = r0;

    asm("syscall");
}