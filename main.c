#include <stdio.h>
#include "include/instruction.h"
#include "include/cpu.h"
#include "include/vm.h"

#define R0 0x00
#define R1 0x01
#define R2 0x02
#define R3 0x03
#define R4 0x04
#define R5 0x05
#define R6 0x06
#define R7 0x07

// uint8_t program[] = {
//     // MOV R0, 0x1234
//     INST_LIT(MOV_16_R_I), REG_LIT(R0), 0x34, 0x12,
//     INST_LIT(MOV_8_R_I), REG_LIT(R1), 'A',
//     INST_LIT(MOV_8_R_R), REG_LIT(R2), REG_LIT(R1),

//     // 0xFF, 0xFF
// };

int main() {

    #define MESSAGE (uint8_t)((int)96 - 9)
    #define READ_BUFFER (uint8_t)((int)96 - 2)

    uint8_t program[] = {
        // MOV R0, 0x1234
        INST_LIT(OP_MOV32RI), REG_LIT(R0), 0x01, 0x00, 0x00, 0x00,              // SET R0 TO 1
        INST_LIT(OP_MOV32RR), REG_LIT(R1), REG_LIT(R0),                         // SET R1 TO R0 (1)
        INST_LIT(OP_MOV32RI), REG_LIT(R2), MESSAGE, 0x00, 0x00, 0x00,           // SET R2 TO MESSAGE
        INST_LIT(OP_MOV32RI), REG_LIT(R3), 0x06, 0x00, 0x00, 0x00,              // SET R3 TO 6

        INST_LIT(OP_SYSCALL),                                                   // PRINT MESSAGE (WRITE SYSCALL)

        INST_LIT(OP_MOV32RI), REG_LIT(R0), 0x00, 0x00, 0x00, 0x00,              // SET R0 TO 0
        INST_LIT(OP_MOV32RI), REG_LIT(R1), 0x00, 0x00, 0x00, 0x00,              // SET R1 TO 0
        INST_LIT(OP_MOV32RI), REG_LIT(R2), READ_BUFFER, 0x00, 0x00, 0x00,       // SET R2 TO READ_BUFFER
        INST_LIT(OP_MOV32RI), REG_LIT(R3), 0x02, 0x00, 0x00, 0x00,              // SET R3 TO 2

        INST_LIT(OP_SYSCALL),                                                   // READ INPUT (READ SYSCALL)

        INST_LIT(OP_LOAD8RI), REG_LIT(R0), READ_BUFFER, 0x00, 0x00, 0x00,       // LOAD READ_BUFFER'S FIRST BYTE INTO R0
        INST_LIT(OP_CMP8RI), REG_LIT(R0), 'K',                                  // COMPARE R0 TO 'K'

        INST_LIT(OP_JNZ32I), 0x00, 0x00, 0x00, 0x00,                            // JUMP TO 0x00 IF NOT EQUAL
        0xFF, 0xFF,                                                             // CRASH THE PROGAM

        'T', 'y', 'p', 'e', ':', ' ', '\0',                                     // MESSAGE
        0x00, 0x00                                                              // READ BUFFER
    };
    

    FILE* debug = fopen("debug.txt", "w");
    fseek(debug, 0, SEEK_SET);

    VM vm;
    vm_init(&vm, 1024);

    vm_load_program(&vm, program, sizeof(program));
    vm_run(&vm, debug);

    cpu_print_registers(&vm.cpu, debug);

    fclose(debug);
    
    return 0;
}