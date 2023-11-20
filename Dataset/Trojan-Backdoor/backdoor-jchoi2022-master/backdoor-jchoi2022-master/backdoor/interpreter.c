// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)
#define HEAP_SZ  (8192)

// Global variable that indicates if the process is running.
static bool is_running = true;

void usageExit(int argc, char **argv) {
  if (argc >= 1)
    printf("%s <bytecode file>\n", argv[0]);
  else
    printf("./interpreter <bytecode file>\n");

  exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

    f[0x00] = halt;
    f[0x10] = load;
    f[0x20] = store;
    f[0x30] = move;
    f[0x40] = puti;
    f[0x50] = add;
    f[0x60] = sub;
    f[0x70] = gt;
    f[0x80] = ge;
    f[0x90] = eq;
    f[0xa0] = ite;
    f[0xb0] = jump;
    f[0xc0] = puts_instr;
    f[0xd0] = gets_instr;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i] = 0;
    }
}

int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode_f;
    uint32_t* code;
    uint8_t heap[HEAP_SZ + 16];
    uint32_t code_size, num_instrs;

    // There should be one argument.
    if (argc != 2) usageExit(argc, argv);

    // Load bytecode file
    bytecode_f = fopen(argv[1], "rb");
    if (bytecode_f == NULL) {
        perror("fopen");
        return 1;
    }

    fseek(bytecode_f, 0L, SEEK_END);
    code_size = ftell(bytecode_f);
    fseek(bytecode_f, 0L, SEEK_SET); // should rewind to start of the file
    if (code_size % 4 != 0 ) {
      printf("%s file size is invalid\n", argv[1]);
      return 1;
    }
    num_instrs = code_size / sizeof(uint32_t);

    code = (uint32_t *) malloc(code_size);
    if (num_instrs != fread(code, sizeof (uint32_t), num_instrs, bytecode_f)) {
      perror("fread");
      return 1;
    }

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f, num_instrs, code, HEAP_SZ, heap);

    while (is_running)
        is_running = stepVMContext(&vm);

    fclose(bytecode_f);

    // Zero indicates normal termination.
    return 0;
}
