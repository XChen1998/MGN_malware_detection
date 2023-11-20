// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
bool is_running = true;

void usageExit() {
	printf("USAGE: ./interpreter [file.mini]");
	exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
	uint32_t i;
	for (i = 0; i < cnt; i++) {
		f[i] = NULL;
	}

	// Initialize function pointers
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
	f[0xc0] = op_puts; 
	f[0xd0] = op_gets;

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
	FILE* bytecode;
	int i = 0;
	uint32_t* pc;

	// There should be at least one argument.
	if (argc < 2) usageExit();

	// Initialize registers.
	initRegs(r, NUM_REGS);

	// Initialize interpretation functions.
	initFuncs(f, NUM_FUNCS);

	// Initialize VM context.
	initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);


	// Load bytecode file
	bytecode = fopen(argv[1], "rb");
	if (bytecode == NULL) {
		perror("fopen");
		return 1;
	}


	while (is_running) {
		// Read 4-byte bytecode, and set the pc accordingly
		printf("Running instruction: %d -> [ '%c', '%d', '%d', '%d' ]\n", i, EXTRACT_B0(*pc), EXTRACT_B1(*pc), EXTRACT_B2(*pc), EXTRACT_B3(*pc));
		stepVMContext(&vm, &pc);
		i++;
	}

	fclose(bytecode);

	return 0;
}
