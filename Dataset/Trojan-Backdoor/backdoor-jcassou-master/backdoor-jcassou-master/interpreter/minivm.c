//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Op1 (ctx->r[EXTRACT_B1(instr)].value)
#define Op2 (ctx->r[EXTRACT_B2(instr)].value)
#define Op3 (ctx->r[EXTRACT_B3(instr)].value) 

extern bool is_running;

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:

void check_addr(uint32_t addr) {
    if (addr >= 8192) {
        perror("addr is out of range");
        exit(1);
    }
}

uint8_t read_mem(struct VMContext* ctx, uint32_t addr) {
    check_addr(addr);
    return *(ctx->heap + addr);
}

void write_mem(struct VMContext* ctx, uint32_t addr, uint8_t cst) {
    check_addr(addr);
    *(ctx->heap + addr) = cst;
}

void halt() {
	is_running = false;
}

void load(struct VMContext* ctx, const uint32_t instr){
	Op1 = ctx->heap[Op2];
}

void store(struct VMContext* ctx, const uint32_t instr){
	ctx->heap[Op1] = Op2;
}


void move(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2;
}

void puti(struct VMContext* ctx, const uint32_t instr){
	Op1 = EXTRACT_B2(instr);
}

void add(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2 + Op3;
}

void sub(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2 - Op3;
}

void gt(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2 > Op3 ? 1 : 0;
}

void ge(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2 >= Op3 ? 1 : 0;
}

void eq(struct VMContext* ctx, const uint32_t instr){
	Op1 = Op2 == Op3 ? 1 : 0;
}

void ite(struct VMContext* ctx, const uint32_t instr){
	uint8_t imm1 = EXTRACT_B2(instr);
	uint8_t imm2 = EXTRACT_B3(instr);
	ctx->pc = (Op1 > 0) ? imm1 : imm2;
}

void jump(struct VMContext* ctx, const uint32_t instr){
	uint8_t imm = EXTRACT_B2(instr);
	ctx->pc = imm - 1;
}

void op_puts(struct VMContext* ctx, const uint32_t instr){
	uint32_t addr = Op1;
	uint8_t cst;
	while(true){
		cst = read_mem(ctx, addr);
		if(cst){
			putchar(cst);
			addr++;
		}
		else
			break;
	}		
}

void op_gets(struct VMContext* ctx, const uint32_t instr){
	uint32_t addr = Op1;
	uint8_t cst;
	while(true) {
		cst = (uint8_t) getchar();
		if (cst == '\n') break;
		write_mem(ctx, addr, cst);
		addr++;
	}
	write_mem(ctx, addr, '\0');
}

// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
	const uint8_t i = EXTRACT_B0(instr);
	(*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
	ctx->numRegs    = numRegs;
	ctx->numFuns    = numFuns;
	ctx->r          = registers;
	ctx->funtable   = funtable;
	ctx->pc         = 0;
	ctx->opcode     = 0;
	memset(ctx->heap, 0, 8192);	
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

