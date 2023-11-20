// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

void usageExit() {
    printf("usage: interpreter <bytecode-file>\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    // TODO: initialize function pointers
    f[0x00] = haltFunction;
    f[0x10] = loadFunction;
    f[0x20] = storeFunction;
    f[0x30] = moveFunction;
    f[0x40] = putiFunction;
    f[0x50] = addFunction;
    f[0x60] = subFunction;
    f[0x70] = gtFunction;
    f[0x80] = geFunction;
    f[0x90] = eqFunction;
    f[0xa0] = iteFunction;
    f[0xb0] = jumpFunction;
    f[0xc0] = putsFunction;
    f[0xd0] = getsFunction;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* fd;
    uint32_t* pc;
    fpos_t filePos;
    size_t fileSize;

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Load bytecode file
    fd = fopen(argv[1], "rb");
    if (fd == NULL) {
        perror("fopen");
        return 1;
    }

    fseek(fd, 0, SEEK_END);
    fgetpos(fd, &filePos);
    fileSize = filePos.__pos;
    pc = (uint32_t*)malloc(fileSize);
    vm.code = pc;
    vm.instr_num = fileSize / 4;

    fseek(fd, 0, SEEK_SET);
    fread(pc, 4, vm.instr_num, fd);

    while (vm.is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly

        if (pc - vm.code >= vm.instr_num)
        {
            puts("\n*****error*****");
            printf("No instuction(%ld)\n", (long unsigned int)(pc - vm.code));
            exit(1);
        }

        stepVMContext(&vm, &pc);
        if (vm.jmp_flag)
        {
            vm.jmp_flag = false;
            pc = vm.code + vm.jmp_value;
        }
    }

    fclose(fd);

    // Zero indicates normal termination.
    return 0;
}
