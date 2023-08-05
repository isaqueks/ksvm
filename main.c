#include <stdio.h>
#include "include/instruction.h"
#include "include/cpu.h"
#include "include/vm.h"


int main(int argc, char** argv) {



    FILE* debug = fopen("debug.txt", "w");
    fseek(debug, 0, SEEK_SET);


    void* prog = NULL;
    int psize = 0;
    void* memory = malloc(1024);

    VM vm;
    vm_init(&vm, memory, 1024);

    if (argc == 2) {
        FILE* file = fopen(argv[1], "rb");
        char* buff = malloc(1024);
        prog = buff;
        psize = fread(buff, 1, 1024, file);
        fclose(file);

        fprintf(debug, "Loaded %d bytes from %s\n", psize, argv[1]);
        vm_dump_program(&vm, buff, psize, debug);
        fprintf(debug, "### END PROGRAM ###\n\n");
        fflush(debug);
    }
    else {
        fprintf(stderr, "No program loaded\n");
        return 1;
    }

    vm_load_program(&vm, prog, psize);
    vm_run(&vm, debug);

    cpu_print_registers(&vm.cpu, debug);

    fclose(debug);
    
    return 0;
}