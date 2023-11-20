//-----------------------------------------------------------------------------
// minivm.h -- Prototypes and definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdint.h>

#ifndef MINIVM_H
#define MINIVM_H


//---------------------------------------------------------
// MACRO DEFINITIONS:


// Size of the global function pointer table
#define MVM_NUM_FUNS 256

#define MVM_NUM_REGISTERS 16 // Default

#define DEFAULT_HEAP_SIZE 8192

//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

struct VMContext;

typedef void (*FunPtr)(struct VMContext* ctx, const uint32_t);

//CHANGE THE INTERNALS OF THIS FOR YOUR OWN VM!
typedef struct Reg {
    uint32_t type;
    uint32_t value;
} Reg;

typedef struct VMContext {
    uint32_t numRegs;
    uint32_t numFuns;
    Reg* r;           // Ptr to register array.
    FunPtr* funtable; // Ptr to a funptr table.

    uint32_t pc; //Program Counter
    uint32_t* code; //The Code DATA
    uint32_t codesize; //Code size
    uint8_t Memory[DEFAULT_HEAP_SIZE]; //Memory
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
void halt(struct VMContext* ctx, const uint32_t instr);
void load(struct VMContext* ctx, const uint32_t instr);
void store(struct VMContext* ctx, const uint32_t instr);
void move(struct VMContext* ctx, const uint32_t instr);
void puti(struct VMContext* ctx, const uint32_t instr);
void add(struct VMContext* ctx, const uint32_t instr);
void sub(struct VMContext* ctx, const uint32_t instr);
void gt(struct VMContext* ctx, const uint32_t instr);
void ge(struct VMContext* ctx, const uint32_t instr);
void eq(struct VMContext* ctx, const uint32_t instr);
void ite(struct VMContext* ctx, const uint32_t instr);
void jump(struct VMContext* ctx, const uint32_t instr);
void _puts(struct VMContext* ctx, const uint32_t instr);
void _gets(struct VMContext* ctx, const uint32_t instr);


// Selects and executes an opcode function from the function pointer table.
// Passes the entire bytecode instruction as the argument.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr);

// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable,
                           uint32_t* code,
                            uint32_t codesize
                             );

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx);


//---------------------------------------------------------
#endif //ifndef MINIVM_H
