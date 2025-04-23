#ifndef INSTRUCTION_H
#define INSTRUCTION_H 1

#include <inttypes.h>

typedef uint16_t InstructionOpcode;

typedef struct {
    InstructionOpcode opcode;
    uint16_t operation_size;
    uint16_t left_operand_size;
    uint16_t right_operand_size;
    
} InstructionDef;


#define MOV_BASE 10
#define ADD_BASE 20
#define SUB_BASE 30
#define MUL_BASE 40
#define DIV_BASE 50
#define CMP_BASE 60
#define JMP_BASE 70
#define JZ_BASE 80
#define JNZ_BASE 90
#define LOAD_BASE 100
#define STORE_BASE 110
#define PUSH_BASE 120
#define POP_BASE 130
#define SYSCALL_BASE 140
#define CALL_BASE 150
#define RET_BASE 160
#define JG_BASE 170
#define JGE_BASE 180
#define JL_BASE 190
#define JLE_BASE 200

typedef enum { 

    OP_MOV32RR = MOV_BASE,
    OP_MOV32RI = MOV_BASE + 1,
    OP_MOV16RR = MOV_BASE + 2,
    OP_MOV16RI = MOV_BASE + 3,
    OP_MOV8RR = MOV_BASE + 4,
    OP_MOV8RI = MOV_BASE + 5,

    OP_ADD32RR = ADD_BASE,
    OP_ADD32RI = ADD_BASE + 1,
    OP_ADD16RR = ADD_BASE + 2,
    OP_ADD16RI = ADD_BASE + 3,
    OP_ADD8RR = ADD_BASE + 4,
    OP_ADD8RI = ADD_BASE + 5,

    OP_SUB32RR = SUB_BASE,
    OP_SUB32RI = SUB_BASE + 1,
    OP_SUB16RR = SUB_BASE + 2,
    OP_SUB16RI = SUB_BASE + 3,
    OP_SUB8RR = SUB_BASE + 4,
    OP_SUB8RI = SUB_BASE + 5,

    OP_MUL32RR = MUL_BASE,
    OP_MUL32RI = MUL_BASE + 1,
    OP_MUL16RR = MUL_BASE + 2,
    OP_MUL16RI = MUL_BASE + 3,
    OP_MUL8RR = MUL_BASE + 4,
    OP_MUL8RI = MUL_BASE + 5,

    OP_DIV32RR = DIV_BASE,
    OP_DIV32RI = DIV_BASE + 1,
    OP_DIV16RR = DIV_BASE + 2,
    OP_DIV16RI = DIV_BASE + 3,
    OP_DIV8RR = DIV_BASE + 4,
    OP_DIV8RI = DIV_BASE + 5,

    OP_CMP32RR = CMP_BASE,
    OP_CMP32RI = CMP_BASE + 1,
    OP_CMP16RR = CMP_BASE + 2,
    OP_CMP16RI = CMP_BASE + 3,
    OP_CMP8RR = CMP_BASE + 4,
    OP_CMP8RI = CMP_BASE + 5,

    OP_JMP32R = JMP_BASE,
    OP_JMP32I = JMP_BASE + 2,

    OP_JZ32R = JZ_BASE,
    OP_JZ32I = JZ_BASE + 2,

    OP_JNZ32R = JNZ_BASE,
    OP_JNZ32I = JNZ_BASE + 2,

    OP_JG32R = JG_BASE,
    OP_JG32I = JG_BASE + 2,

    OP_JGE32R = JGE_BASE,
    OP_JGE32I = JGE_BASE + 2,

    OP_JL32R = JL_BASE,
    OP_JL32I = JL_BASE + 2,

    OP_JLE32R = JLE_BASE,
    OP_JLE32I = JLE_BASE + 2,

    OP_LOAD32RR = LOAD_BASE,
    OP_LOAD32RI = LOAD_BASE + 1,
    OP_LOAD16RR = LOAD_BASE + 2,
    OP_LOAD16RI = LOAD_BASE + 3,
    OP_LOAD8RR = LOAD_BASE + 4,
    OP_LOAD8RI = LOAD_BASE + 5,

    OP_STORE32RR = STORE_BASE,
    OP_STORE32RI = STORE_BASE + 1,
    OP_STORE16RR = STORE_BASE + 2,
    OP_STORE16RI = STORE_BASE + 3,
    OP_STORE8RR = STORE_BASE + 4,
    OP_STORE8RI = STORE_BASE + 5,

    OP_PUSH32R = PUSH_BASE,
    OP_PUSH32I = PUSH_BASE + 1,
    OP_PUSH16R = PUSH_BASE + 2,
    OP_PUSH16I = PUSH_BASE + 3,
    OP_PUSH8R = PUSH_BASE + 4,
    OP_PUSH8I = PUSH_BASE + 5,

    OP_POP32R = POP_BASE,
    OP_POP16R = POP_BASE + 1,
    OP_POP8R = POP_BASE + 2,

    OP_CALL32R = CALL_BASE,
    OP_CALL32I = CALL_BASE + 1,

    OP_RET = RET_BASE,

    OP_SYSCALL = SYSCALL_BASE,

} Opcodes;

int instruction_get_size(InstructionDef* instr);

InstructionDef* instruction_get_def(InstructionOpcode opcode);

#endif