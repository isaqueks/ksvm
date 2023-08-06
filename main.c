#include <stdio.h>
#include "include/instruction.h"
#include "include/cpu.h"
#include "include/vm.h"
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define DEBUG_PAGE_SIZE 92

bool print_reg = false;

void debug_step(VM* vm, int pc) {
    system("clear");

    int start = pc - (DEBUG_PAGE_SIZE / 2);
    int end = pc + (DEBUG_PAGE_SIZE / 2);

    if (end > vm->memory_size) {
        start = vm->memory_size - DEBUG_PAGE_SIZE;
        end = vm->memory_size;
    }
    else if (start < 0) {
        start = 0;
        end = DEBUG_PAGE_SIZE;
    }

    vm_dump_program(vm, vm->memory, start, end, pc, stdout);
    if (print_reg) {
        printf("\n");
        cpu_print_registers(&vm->cpu, stdout);
    }
    getchar();
}

int main(int argc, char** argv) {



    FILE* debug = fopen("backlog.txt", "w");
    fseek(debug, 0, SEEK_SET);


    void* prog = NULL;
    int psize = 0;
    void* memory = malloc(1024);

    VM vm;
    vm_init(&vm, memory, 1024);

    if (argc >= 2) {
        FILE* file = fopen(argv[1], "rb");
        char* buff = malloc(1024);
        prog = buff;
        psize = fread(buff, 1, 1024, file);
        fclose(file);

        // fprintf(debug, "Loaded %d bytes from %s\n", psize, argv[1]);
        // vm_dump_program(&vm, buff, 0, psize, -1, debug);
        // fprintf(debug, "### END PROGRAM ###\n\n");
        // fflush(debug);
    }
    else {
        fprintf(stderr, "No program loaded\n");
        return 1;
    }

    print_reg = argc == 3 && !strcmp(argv[2], "sr");

    vm_load_program(&vm, prog, psize);
    vm_run(&vm, debug, argc == 3 && argv[2][0] == 's' ? debug_step : NULL);

    cpu_print_registers(&vm.cpu, debug);

    fclose(debug);
    
    return 0;
}