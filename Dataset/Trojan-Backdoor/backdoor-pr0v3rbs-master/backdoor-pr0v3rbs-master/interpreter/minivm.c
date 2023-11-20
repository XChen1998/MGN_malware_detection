//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "minivm.h"

#define HEAP_SIZE 8192

//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:
bool haltFunction(struct VMContext* ctx, __attribute__((unused)) const uint32_t instr)
{
    ctx->is_running = false;

    return true;
}

bool loadFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);

    if (ctx->r[r1].value >= HEAP_SIZE)
    {
        heapError(ctx->r[r1].value, r1);
        return false;
    }

    ctx->r[r0].value = 0;
    ctx->r[r0].value = *(uint8_t*)(ctx->heap + ctx->r[r1].value);

    return true;
}

bool storeFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);

    if (ctx->r[r0].value >= HEAP_SIZE)
    {
        heapError(ctx->r[r0].value, r0);
        return false;
    }

    *(uint8_t*)(ctx->heap + ctx->r[r0].value) = EXTRACT_B0(ctx->r[r1].value);

    return true;
}


bool moveFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    ctx->r[r0].value = ctx->r[r1].value;

    return true;
}

bool putiFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t imm = EXTRACT_B2(instr);
    ctx->r[r0].value = (uint32_t)imm;

    return true;
}

bool addFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    const uint8_t r2 = EXTRACT_B3(instr);
    ctx->r[r0].value = ctx->r[r1].value + ctx->r[r2].value;

    return true;
}

bool subFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    const uint8_t r2 = EXTRACT_B3(instr);
    ctx->r[r0].value = ctx->r[r1].value - ctx->r[r2].value;

    return true;
}

bool gtFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    const uint8_t r2 = EXTRACT_B3(instr);
    if (ctx->r[r1].value > ctx->r[r2].value)
        ctx->r[r0].value = 1;
    else
        ctx->r[r0].value = 0;

    return true;
}

bool geFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    const uint8_t r2 = EXTRACT_B3(instr);
    if (ctx->r[r1].value >= ctx->r[r2].value)
        ctx->r[r0].value = 1;
    else
        ctx->r[r0].value = 0;

    return true;
}

bool eqFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    const uint8_t r1 = EXTRACT_B2(instr);
    const uint8_t r2 = EXTRACT_B3(instr);
    if (ctx->r[r1].value == ctx->r[r2].value)
        ctx->r[r0].value = 1;
    else
        ctx->r[r0].value = 0;

    return true;
}

bool iteFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);

    if (ctx->r[r0].value > 0)
    {
        ctx->jmp_flag = true;
        ctx->jmp_value = EXTRACT_B2(instr);
    }
    else if (ctx->r[r0].value == 0)
    {
        ctx->jmp_flag = true;
        ctx->jmp_value = EXTRACT_B3(instr);
    }

    return true;
}

bool jumpFunction(struct VMContext* ctx, const uint32_t instr)
{
    ctx->jmp_flag = true;
    ctx->jmp_value = EXTRACT_B1(instr);

    return true;
}

bool putsFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);

    if (ctx->r[r0].value >= HEAP_SIZE)
    {
        heapError(ctx->r[r0].value, r0);
        return false;
    }

    printf("%s", (char*)(ctx->heap + ctx->r[r0].value));

    return true;
}

bool getsFunction(struct VMContext* ctx, const uint32_t instr)
{
    const uint8_t r0 = EXTRACT_B1(instr);
    char* str = (char*)(ctx->heap + ctx->r[r0].value);
    uint32_t heapHardAddr = ctx->r[r0].value;

    while (true)
    {
        if (heapHardAddr >= HEAP_SIZE)
        {
            heapError(heapHardAddr, r0);
            return false;
        }

        *str = getc(stdin);
        if (*str == '\n')
        {
            *(str) = '\0';
            break;
        }
        str++;
        heapHardAddr++;
    }

    return true;
}


// Defers decoding of register args to the called function.
// dispatch :: VMContext -> uint32_t -> Effect()
bool dispatch(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t i = EXTRACT_B0(instr);
    if ((*ctx->funtable[i]) == NULL)
    {
        puts("\n*****error*****");
	printf("unknown opcode number(0x%x) ", i);
        return false;
    }

    return (*ctx->funtable[i])(ctx, instr);
}


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns, Reg* registers, FunPtr* funtable) {
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->instr_num  = 0;
    ctx->code       = NULL;
    ctx->is_running = true;
    ctx->jmp_flag   = false;
    ctx->jmp_value  = 0;
    ctx->heap       = malloc(HEAP_SIZE);
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> uint32_t** -> Effect()
void stepVMContext(struct VMContext* ctx, uint32_t** pc) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr = **pc;

    // Dispatch to an opcode-handler.
    if (dispatch(ctx, instr) == false)
    {
        printf("on %ld instruction\n", *pc - ctx->code);
        exit(1);
    }

    // Increment to next instruction.
    (*pc)++;
}

void heapError(uint32_t address, uint8_t regNum)
{
    puts("\n*****error*****");
    printf("illegal heap address 0x%08x at %d register ", address, regNum);
}
