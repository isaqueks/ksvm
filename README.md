# KSVM - A simple 32-bit virtual machine written in C  

```
 ##     00 01 02 03 04 05 06 07                 INST            OP1             OP2                     ASCII

 0000   0B 00 00 00 99 00 00 00                 MOV32           EAX             00000099
 0008   78 00 00 00 .. .. .. ..                 PUSH32          EAX
 000C   78 00 1C 00 .. .. .. ..                 PUSH32          EIP
 0010   48 00 59 00 00 00 .. ..                 JMP32           00000059
[0016]  FF FF .. .. .. .. .. ..                 [????                                                   ..]
 0018   82 00 02 00 .. .. .. ..                 POP32           ECX
 001C   15 00 02 00 06 00 00 00                 ADD32           ECX             00000006
 0024   0A 00 03 00 00 00 .. ..                 MOV32           EDX             EAX
 002A   68 00 01 00 00 00 .. ..                 LOAD8           EBX             EAX
 0030   41 00 01 00 00 .. .. ..                 CMP8            EBX             00
 0035   15 00 00 00 01 00 00 00                 ADD32           EAX             00000001
 003D   5C 00 2A 00 00 00 .. ..                 JNZ32           0000002A
 0043   1E 00 00 00 03 00 .. ..                 SUB32           EAX             EDX
 0049   15 00 00 00 01 00 00 00                 ADD32           EAX             00000001
 0051   78 00 00 00 .. .. .. ..                 PUSH32          EAX
 0055   46 00 02 00 .. .. .. ..                 JMP32           ECX
 0059   82 00 0B 00 .. .. .. ..                 POP32           R11
 005D   82 00 0A 00 .. .. .. ..                 POP32           R10
 0061   0A 00 00 00 0A 00 .. ..                 MOV32           EAX             R10
 0067   78 00 1C 00 .. .. .. ..                 PUSH32          EIP
 006B   48 00 18 00 00 00 .. ..                 JMP32           00000018
 0071   82 00 03 00 .. .. .. ..                 POP32           EDX
 0075   0B 00 00 00 01 00 00 00                 MOV32           EAX             00000001
 007D   0B 00 01 00 01 00 00 00                 MOV32           EBX             00000001
```

## Introduction  

Purpose: It's just a project I made for fun. There's probably no practical use for it.    
It's a 32-bit machine with 32 registers. They are:
```
EAX (R0)
EBX (R1)
ECX (R2)
EDX (R3)
R4 - R25
ESP (Stack pointer)
EBP (Stack base pointer)
EIP (Instruction pointer)
FLAGS
R30 - R31 (Reserved)
```

## Instruction format
Instructions follow the following format:  
```
[OPCODE] [OP1 (R/I)] [OP2 (R/I)]
```
Each OPCODE uses 2 bytes, R (register) OP uses 2 bytes and I (immediate) OP uses SIZE / 8 bytes, where SIZE is how many bits will be written/read.   
The opcodes are named as follows:  
```
[NAME][SIZE][R/I]?[R/I]?
```
Where NAME is the name of the instruction, SIZE is the size of the operation in bits and R/I is whether the operation uses a register or an immediate value. Some instructions can use 2, 1 and no operands at all. SIZE can be 8, 16 or 32. Some instructions can only be used with 32 SIZE, like JMP32R/I as the address to jump to can only be 32 bits long.   

### Instruction list
Check `instructions.h` for the full list of instructions.

### Usage
(You probably would want to change `main.c` to your needs) `<compiled binary> <binary file> s?r?`:
The binary file is the program the VM will run, `s` is for step-by-step mode and `r` is for printing the registers after each instruction. The step mode will dump the memory near `EIP` and highlight the current instruction.       
The file `backlog.txt` will be generated and written to after each instruction. It will contain the current executed instruction, and the final registers state.    

### System calls
If you check, there is a `SYSCALL` instruction. It uses native x64 system calls. Some syscalls might not work now because I added address translation to only a few instructions, I mean, before calling a natove syscall, the VM needs to translate the program's memory addresses to the host's memory addresses. I will add more translations in the future.

### Assembler
I made a simple assembler in python, the source code is in assembler directory. It is very simple (and probably very buggy) at the moment, I will write a better one in the future.     
You can use it like this: `python3 asm.py <input file> <output file>`

### Examples
(Check `examples` directory)    
*ask_name.ksasm*
```asm
; Simple application to ask the user's name and print it back

; _START isn't really the application entry point is the first byte. 
; Just for tradition:
_START:
    MOV32RI EAX :MESSAGE
    MOV32RI EBX 16

    CALL32I :PRINT

    MOV32RI EAX :BUFFER
    MOV32RI EBX 16

    CALL32I :READLINE

    MOV32RI EAX :MESSAGE_2
    MOV32RI EBX 13

    CALL32I :PRINT

    MOV32RI EAX :BUFFER
    MOV32RI EBX 16

    CALL32I :PRINT

    $ xFF xFF


PRINT: ; (EAX = MESSAGE, EBX = LEN)
    MOV32RR ECX EAX
    MOV32RR EDX EBX
    MOV32RI EAX x01
    MOV32RI EBX x01

    SYSCALL

    RET

READLINE: ; (EAX = BUFFER, EBX = LEN)
    MOV32RR ECX EAX
    MOV32RR EDX EBX
    MOV32RI EAX x00
    MOV32RI EBX x00

    SYSCALL

    RET

MESSAGE:
$ "Type your name: " x00
MESSAGE_2:
$ "Your name is " x00
BUFFER:
$ "               " x00
```

### Todos
* Add more instructions (like JGE, ...)
* Add more syscalls's address translations
* Add memory paging
* Improve debugging
* Write a better assembler