//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "minivm.h"

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:

void error_h(int v)
{
	printf("\n\n***Error: ");
	if (v == HeapError) printf("Invalid heap memory address\n");
	else if (v == OpcodeError) printf("Invalid opcode\n");
	else if (v == IpError) printf("Invalid instruction pointer\n");
	else if (v == InputError) printf("Invalid input\n");
	else printf("Unknown\n");
	exit(1);
}

// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
	const uint8_t i = EXTRACT_B0(instr);
	if (ctx->funtable[i] == NULL)
	{
		error_h(OpcodeError);
	}
	(*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
	ctx->numRegs = numRegs;
	ctx->numFuns = numFuns;
	ctx->r = registers;
	ctx->funtable = funtable;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc) {
	// Read a 32-bit bytecode instruction.
	uint32_t instr = **pc;

	// Dispatch to an opcode-handler.
	dispatch(ctx, instr);

	// Increment to next instruction.
	(*pc)++;
}

