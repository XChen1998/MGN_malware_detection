//-----------------------------------------------------------------------------
// minivm.h -- Prototypes and definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdlib.h>

#ifndef MINIVM_H
#define MINIVM_H


//---------------------------------------------------------
// MACRO DEFINITIONS:


// Size of the global function pointer table
#define MVM_NUM_FUNS 256

#define MVM_NUM_REGISTERS 16 // Default


//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

struct VMContext;

typedef void (*FunPtr)(struct VMContext* ctx, const uint32_t);

//CHANGE THE INTERNALS OF THIS FOR YOUR OWN VM!
typedef struct Reg {
    uint32_t value;
} Reg;

typedef struct VMContext {
    uint32_t numRegs;
    uint32_t numFuns;
    uint32_t memSize;
    uint32_t codeSize;
    Reg* r;           // Ptr to register array.
    FunPtr* funtable; // Ptr to a funptr table.
    uint8_t* mem;
    uint32_t* code;
    uint32_t pc;
} VMContext;


//---------------------------------------------------------
// ESOTERIC ITEMS:


#ifdef MVM_GLOBAL_FUNTABLE
// The global function pointer table.
static FunPtr mvm_function_table[MVM_NUM_FUNS];
#endif

// Byte extraction macros.
#define EXTRACT_B0(i) (i & 0xFF)
#define EXTRACT_B1(i) ((i >> 8) & 0xFF)
#define EXTRACT_B2(i) ((i >> 16) & 0xFF)
#define EXTRACT_B3(i) ((i >> 24) & 0xFF)


//---------------------------------------------------------
// FUNCTIONS:


// Selects and executes an opcode function from the function pointer table.
// Passes the entire bytecode instruction as the argument.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr);

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
                           uint32_t* code);

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx);


//---------------------------------------------------------
#endif //ifndef MINIVM_H
