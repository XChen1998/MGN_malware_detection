//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    (*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                      const uint32_t memSize,
                         const off_t codeSize,
                                Reg* registers,
                             FunPtr* funtable,
                            uint8_t* mem,
                           uint32_t* code) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->memSize    = memSize;
    ctx->codeSize   = codeSize;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->mem        = mem;
    ctx->code       = code;
    ctx->pc         = 0;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx) {
    uint32_t instr;
    // Check validity of pc
    if (ctx->pc >= ctx->codeSize) {
        exit(-1);
    }
    // Read a 32-bit bytecode instruction.
    instr = ctx->code[ctx->pc];

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);
}

