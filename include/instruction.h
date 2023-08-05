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

// #define DECLARE_INSTRUCTION_0_0(name, code) \
//     const InstructionOpcode OP_##name = code; \
//     const InstructionDef name = { \
//         code, \
//         0, \
//         0, \
//         0 \
//     }

// #define DECLARE_INSTRUCTION_R(name, size, code) \
//     const InstructionOpcode OP_##name##_##size##_##R = code; \
//     const InstructionDef name##_##size##_##R = { \
//         code, \
//         size / 8, \
//         2, \
//         0 \
//     }

// #define DECLARE_INSTRUCTION_I(name, size, code) \
//     const InstructionOpcode OP_##name##_##size##_##I = code; \
//     const InstructionDef name##_##size##_##I = { \
//         code, \
//         size / 8, \
//         size / 8, \
//         0 \
//     }

// #define DECLARE_INSTRUCTION_R_I(name, size, code) \
//     const InstructionOpcode OP_##name##_##size##_##R_I = code; \
//     const InstructionDef name##_##size##_##R_I = { \
//         code, \
//         size / 8, \
//         2, \
//         size / 8 \
//     }

// #define DECLARE_INSTRUCTION_R_R(name, size, code) \
//     const InstructionOpcode OP_##name##_##size##_##R_R = code; \
//     const InstructionDef name##_##size##_##R_R = { \
//         code, \
//         size / 8, \
//         2, \
//         2 \
//     }


// #define DECLARE_INSTRUCTION_R_X(name, size, op) \
//     DECLARE_INSTRUCTION_R_I(name, size, op); \
//     DECLARE_INSTRUCTION_R_R(name, size, op + 1);

// #define DECLARE_INSTRUCTION_X(name, size, op) \
//     DECLARE_INSTRUCTION_R(name, size, op); \
//     DECLARE_INSTRUCTION_I(name, size, op + 1);



// #define INSTRUCTION_S_R_X(name, op) \
//     DECLARE_INSTRUCTION_R_X(name, 32, op); \
//     DECLARE_INSTRUCTION_R_X(name, 16, op + 2); \
//     DECLARE_INSTRUCTION_R_X(name, 8, op + 4);
//     // const InstructionOpcode name##_32_R_R = op, \
//     // name##_32_R_I = op + 1, \
//     // name##_32_I_I = op + 2, \
//     // name##_16_R_R = op + 3, \
//     // name##_16_R_I = op + 4, \
//     // name##_16_I_I = op + 5, \
//     // name##_8_R_R = op + 6, \
//     // name##_8_R_I = op + 7, \
//     // name##_8_I_I = op + 8;

// // #define INSTRUCTION_S_R_X(name, op) \
// //     DECLARE_INSTRUCTION_R_X(name, 32, op); \
// //     DECLARE_INSTRUCTION_R_X(name, 16, op + 1); \
// //     DECLARE_INSTRUCTION_R_X(name, 8, op + 2);
//     // const InstructionOpcode name##_32_R_R = op, \
//     // name##_32_R_I = op + 1, \
//     // name##_16_R_R = op + 2, \
//     // name##_16_R_I = op + 3, \
//     // name##_8_R_R = op + 4, \
//     // name##_8_R_I = op + 5;


// #define INSTRUCTION_S_R(name, op) \
//     DECLARE_INSTRUCTION_R(name, 32, op); \
//     DECLARE_INSTRUCTION_R(name, 16, op + 1); \
//     DECLARE_INSTRUCTION_R(name, 8, op + 2);
//     // const InstructionOpcode name##_32_R = op, \
//     // name##_16_R = op + 1, \
//     // name##_8_R = op + 2;

// #define INSTRUCTION_S_I(name, op) \
//     DECLARE_INSTRUCTION_I(name, 32, op); \
//     DECLARE_INSTRUCTION_I(name, 16, op + 1); \
//     DECLARE_INSTRUCTION_I(name, 8, op + 2);
//     // const InstructionOpcode name##_32_I = op, \
//     // name##_16_I = op + 1, \
//     // name##_8_I = op + 2;

// #define INSTRUCTION_S_X(name, op) \
//     DECLARE_INSTRUCTION_R(name, 32, op); \
//     DECLARE_INSTRUCTION_R(name, 16, op + 1); \
//     DECLARE_INSTRUCTION_R(name, 8, op + 2); \
//     DECLARE_INSTRUCTION_I(name, 32, op + 3); \
//     DECLARE_INSTRUCTION_I(name, 16, op + 4); \
//     DECLARE_INSTRUCTION_I(name, 8, op + 5);
    


// #define INSTRUCTION_32_R(name, op) \
//     DECLARE_INSTRUCTION_R(name, 32, op);
//     // const InstructionOpcode name##_32_R = op;

// #define INSTRUCTION_32_I(name, op) \
//     DECLARE_INSTRUCTION_I(name, 32, op);
//     // const InstructionOpcode name##_32_I = op;

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

// typedef enum {

    // INSTRUCTION_S_R_X(MOV, MOV_BASE);
    // INSTRUCTION_S_R_X(ADD, ADD_BASE);
    // INSTRUCTION_S_R_X(SUB, SUB_BASE);
    // INSTRUCTION_S_R_X(MUL, MUL_BASE);
    // INSTRUCTION_S_R_X(DIV, DIV_BASE);
    // INSTRUCTION_S_R_X(CMP, CMP_BASE);

    // INSTRUCTION_32_R(JMP, JMP_BASE);
    // INSTRUCTION_32_I(JMP, JMP_BASE + 1);

    // // JMP_32_R = JMP_BASE,
    // // JMP_32_I = JMP_BASE + 1,

    // INSTRUCTION_32_R(JZ, JZ_BASE);
    // INSTRUCTION_32_I(JZ, JZ_BASE + 1);

    // // JZ_32_R = JZ_BASE,
    // // JZ_32_I = JZ_BASE + 1,

    // INSTRUCTION_32_R(JNZ, JNZ_BASE);
    // INSTRUCTION_32_I(JNZ, JNZ_BASE + 1);

    // // JNZ_32_R = JNZ_BASE,
    // // JNZ_32_I = JNZ_BASE + 1,

    // INSTRUCTION_S_R_X(LOAD, LOAD_BASE);
    // INSTRUCTION_S_R_X(STORE, STORE_BASE);

    // INSTRUCTION_S_X(PUSH, PUSH_BASE);
    // INSTRUCTION_S_R(POP, POP_BASE);


    // DECLARE_INSTRUCTION_0_0(SYSCALL, SYSCALL_BASE);
    // // SYSCALL = SYSCALL_BASE,


// } Opcodes;

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

    OP_SYSCALL = SYSCALL_BASE,

} Opcodes;

int instruction_get_size(InstructionDef* instr);

InstructionDef* instruction_get_def(InstructionOpcode opcode);

#define INST_LIT(I) I, 0x00

#endif