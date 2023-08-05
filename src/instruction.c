#include <stdlib.h>
#include "../include/instruction.h"

int instruction_get_size(InstructionDef* instr) {
    return 2 + instr->left_operand_size + instr->right_operand_size;
}

const InstructionDef MOV32RR = { OP_MOV32RR, 32, 2, 2 };
const InstructionDef MOV32RI = { OP_MOV32RI, 32, 2, 4 };
const InstructionDef MOV16RR = { OP_MOV16RR, 16, 2, 2 };
const InstructionDef MOV16RI = { OP_MOV16RI, 16, 2, 2 };
const InstructionDef MOV8RR = { OP_MOV8RR, 8, 2, 2 };
const InstructionDef MOV8RI = { OP_MOV8RI, 8, 2, 1 };

const InstructionDef ADD32RR = { OP_ADD32RR, 32, 2, 2 };
const InstructionDef ADD32RI = { OP_ADD32RI, 32, 2, 4 };
const InstructionDef ADD16RR = { OP_ADD16RR, 16, 2, 2 };
const InstructionDef ADD16RI = { OP_ADD16RI, 16, 2, 2 };
const InstructionDef ADD8RR = { OP_ADD8RR, 8, 2, 2 };
const InstructionDef ADD8RI = { OP_ADD8RI, 8, 2, 1 };

const InstructionDef SUB32RR = { OP_SUB32RR, 32, 2, 2 };
const InstructionDef SUB32RI = { OP_SUB32RI, 32, 2, 4 };
const InstructionDef SUB16RR = { OP_SUB16RR, 16, 2, 2 };
const InstructionDef SUB16RI = { OP_SUB16RI, 16, 2, 2 };
const InstructionDef SUB8RR = { OP_SUB8RR, 8, 2, 2 };
const InstructionDef SUB8RI = { OP_SUB8RI, 8, 2, 1 };

const InstructionDef MUL32RR = { OP_MUL32RR, 32, 2, 2 };
const InstructionDef MUL32RI = { OP_MUL32RI, 32, 2, 4 };
const InstructionDef MUL16RR = { OP_MUL16RR, 16, 2, 2 };
const InstructionDef MUL16RI = { OP_MUL16RI, 16, 2, 2 };
const InstructionDef MUL8RR = { OP_MUL8RR, 8, 2, 2 };
const InstructionDef MUL8RI = { OP_MUL8RI, 8, 2, 1 };

const InstructionDef DIV32RR = { OP_DIV32RR, 32, 2, 2 };
const InstructionDef DIV32RI = { OP_DIV32RI, 32, 2, 4 };
const InstructionDef DIV16RR = { OP_DIV16RR, 16, 2, 2 };
const InstructionDef DIV16RI = { OP_DIV16RI, 16, 2, 2 };
const InstructionDef DIV8RR = { OP_DIV8RR, 8, 2, 2 };
const InstructionDef DIV8RI = { OP_DIV8RI, 8, 2, 1 };

const InstructionDef CMP32RR = { OP_CMP32RR, 32, 2, 2 };
const InstructionDef CMP32RI = { OP_CMP32RI, 32, 2, 4 };
const InstructionDef CMP16RR = { OP_CMP16RR, 16, 2, 2 };
const InstructionDef CMP16RI = { OP_CMP16RI, 16, 2, 2 };
const InstructionDef CMP8RR = { OP_CMP8RR, 8, 2, 2 };
const InstructionDef CMP8RI = { OP_CMP8RI, 8, 2, 1 };

const InstructionDef JMP32R = { OP_JMP32R, 32, 2, 0 };
const InstructionDef JMP32I = { OP_JMP32I, 32, 4, 0 };

const InstructionDef JZ32R = { OP_JZ32R, 32, 2, 0 };
const InstructionDef JZ32I = { OP_JZ32I, 32, 4, 0 };

const InstructionDef JNZ32R = { OP_JNZ32R, 32, 2, 0 };
const InstructionDef JNZ32I = { OP_JNZ32I, 32, 4, 0 };

const InstructionDef LOAD32RR = { OP_LOAD32RR, 32, 2, 2 };
const InstructionDef LOAD32RI = { OP_LOAD32RI, 32, 2, 4 };
const InstructionDef LOAD16RR = { OP_LOAD16RR, 16, 2, 2 };
const InstructionDef LOAD16RI = { OP_LOAD16RI, 16, 2, 4 };
const InstructionDef LOAD8RR = { OP_LOAD8RR, 8, 2, 2 };
const InstructionDef LOAD8RI = { OP_LOAD8RI, 8, 2, 4 };

const InstructionDef STORE32RR = { OP_STORE32RR, 32, 2, 2 };
const InstructionDef STORE32RI = { OP_STORE32RI, 32, 2, 4 };
const InstructionDef STORE16RR = { OP_STORE16RR, 16, 2, 2 };
const InstructionDef STORE16RI = { OP_STORE16RI, 16, 2, 2 };
const InstructionDef STORE8RR = { OP_STORE8RR, 8, 2, 2 };
const InstructionDef STORE8RI = { OP_STORE8RI, 8, 2, 1 };

const InstructionDef PUSH32R = { OP_PUSH32R, 32, 2, 0 };
const InstructionDef PUSH32I = { OP_PUSH32I, 32, 4, 0 };
const InstructionDef PUSH16R = { OP_PUSH16R, 16, 2, 0 };
const InstructionDef PUSH16I = { OP_PUSH16I, 16, 2, 0 };
const InstructionDef PUSH8R = { OP_PUSH8R, 8, 2, 0 };
const InstructionDef PUSH8I = { OP_PUSH8I, 8, 1, 0 };

const InstructionDef POP32R = { OP_POP32R, 32, 2, 0 };
const InstructionDef POP16R = { OP_POP16R, 16, 2, 0 };
const InstructionDef POP8R = { OP_POP8R, 8, 2, 0 };

const InstructionDef SYSCALL = { OP_SYSCALL, 32, 0, 0 };

InstructionDef* instruction_get_def(InstructionOpcode opcode) {
    switch (opcode) {
        case OP_MOV32RR:
            return &MOV32RR;
        case OP_MOV32RI:
            return &MOV32RI;
        case OP_MOV16RR:
            return &MOV16RR;
        case OP_MOV16RI:
            return &MOV16RI;
        case OP_MOV8RR:
            return &MOV8RR;
        case OP_MOV8RI:
            return &MOV8RI;

        case OP_ADD32RR:
            return &ADD32RR;
        case OP_ADD32RI:
            return &ADD32RI;
        case OP_ADD16RR:
            return &ADD16RR;
        case OP_ADD16RI:
            return &ADD16RI;
        case OP_ADD8RR:
            return &ADD8RR;
        case OP_ADD8RI:
            return &ADD8RI;

        case OP_SUB32RR:
            return &SUB32RR;
        case OP_SUB32RI:
            return &SUB32RI;
        case OP_SUB16RR:
            return &SUB16RR;
        case OP_SUB16RI:
            return &SUB16RI;
        case OP_SUB8RR:
            return &SUB8RR;
        case OP_SUB8RI:
            return &SUB8RI;

        case OP_MUL32RR:
            return &MUL32RR;
        case OP_MUL32RI:
            return &MUL32RI;
        case OP_MUL16RR:
            return &MUL16RR;
        case OP_MUL16RI:
            return &MUL16RI;
        case OP_MUL8RR:
            return &MUL8RR;
        case OP_MUL8RI:
            return &MUL8RI;

        case OP_DIV32RR:
            return &DIV32RR;
        case OP_DIV32RI:
            return &DIV32RI;
        case OP_DIV16RR:
            return &DIV16RR;
        case OP_DIV16RI:
            return &DIV16RI;
        case OP_DIV8RR:
            return &DIV8RR;
        case OP_DIV8RI:
            return &DIV8RI;

        case OP_CMP32RR:
            return &CMP32RR;
        case OP_CMP32RI:
            return &CMP32RI;
        case OP_CMP16RR:
            return &CMP16RR;
        case OP_CMP16RI:
            return &CMP16RI;
        case OP_CMP8RR:
            return &CMP8RR;
        case OP_CMP8RI:
            return &CMP8RI;

        case OP_JMP32R:
            return &JMP32R;
        case OP_JMP32I:
            return &JMP32I;

        case OP_JZ32R:
            return &JZ32R;
        case OP_JZ32I:
            return &JZ32I;

        case OP_JNZ32R:
            return &JNZ32R;
        case OP_JNZ32I:
            return &JNZ32I;

        case OP_LOAD32RR:
            return &LOAD32RR;
        case OP_LOAD32RI:
            return &LOAD32RI;
        case OP_LOAD16RR:
            return &LOAD16RR;
        case OP_LOAD16RI:
            return &LOAD16RI;
        case OP_LOAD8RR:
            return &LOAD8RR;
        case OP_LOAD8RI:
            return &LOAD8RI;

        case OP_STORE32RR:
            return &STORE32RR;
        case OP_STORE32RI:
            return &STORE32RI;
        case OP_STORE16RR:
            return &STORE16RR;
        case OP_STORE16RI:
            return &STORE16RI;
        case OP_STORE8RR:
            return &STORE8RR;
        case OP_STORE8RI:
            return &STORE8RI;

        case OP_PUSH32R:
            return &PUSH32R;
        case OP_PUSH32I:
            return &PUSH32I;
        case OP_PUSH16R:
            return &PUSH16R;
        case OP_PUSH16I:
            return &PUSH16I;
        case OP_PUSH8R:
            return &PUSH8R;
        case OP_PUSH8I:
            return &PUSH8I;

        case OP_POP32R:
            return &POP32R;
        case OP_POP16R:
            return &POP16R;
        case OP_POP8R:
            return &POP8R;

        case OP_SYSCALL:
            return &SYSCALL;

        default:
            return NULL;
    }
}
