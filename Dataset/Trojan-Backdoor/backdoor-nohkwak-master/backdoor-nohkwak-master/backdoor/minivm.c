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
    if ( i == 0x00 | i == 0x10 | i == 0x20 | i == 0x30 |
         i == 0x40 | i == 0x50 | i == 0x60 | i == 0x70 |
         i == 0x80 | i == 0x90 | i == 0xa0 | i == 0xb0 |
         i == 0xc0 | i == 0xd0 ) {
        (*ctx->funtable[i])(ctx, instr);    
    }
    else {
        // terminating if it encounters an unknown opcode
        printf( "\n\n\n unknown opcode....\n" );     
        (*ctx->funtable[0])(ctx, instr);    
    }
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
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

