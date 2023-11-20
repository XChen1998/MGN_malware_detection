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


//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

struct VMContext;

typedef bool (*FunPtr)(struct VMContext* ctx, const uint32_t);

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
    uint32_t* code;
    uint32_t instr_num;
    bool is_running;  // halt check value.
    bool jmp_flag;
    uint8_t jmp_value;
    void* heap;       // data section base address.
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
bool haltFunction(struct VMContext* ctx, __attribute__((unused)) const uint32_t instr);
bool loadFunction(struct VMContext* ctx, const uint32_t instr);
bool storeFunction(struct VMContext* ctx, const uint32_t instr);
bool moveFunction(struct VMContext* ctx, const uint32_t instr);
bool putiFunction(struct VMContext* ctx, const uint32_t instr);
bool addFunction(struct VMContext* ctx, const uint32_t instr);
bool subFunction(struct VMContext* ctx, const uint32_t instr);
bool gtFunction(struct VMContext* ctx, const uint32_t instr);
bool geFunction(struct VMContext* ctx, const uint32_t instr);
bool eqFunction(struct VMContext* ctx, const uint32_t instr);
bool iteFunction(struct VMContext* ctx, const uint32_t instr);
bool jumpFunction(struct VMContext* ctx, const uint32_t instr);
bool putsFunction(struct VMContext* ctx, const uint32_t instr);
bool getsFunction(struct VMContext* ctx, const uint32_t instr);

// Selects and executes an opcode function from the function pointer table.
// Passes the entire bytecode instruction as the argument.
// dispatch :: VMContext -> uint32_t -> Effect()
bool dispatch(struct VMContext* ctx, const uint32_t instr);

// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx,
                      const uint32_t numRegs,
                      const uint32_t numFuns,
                                Reg* registers,
                             FunPtr* funtable);

// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc);

void heapError(uint32_t address, uint8_t regNum);

//---------------------------------------------------------
#endif //ifndef MINIVM_H
