//-----------------------------------------------------------------------------
// minivm.c -- The basic definitions for the vm.
// Copyright (C) Philip Conrad 4/30/2013 @ 3:56 PM -- MIT License
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "minivm.h"


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:


// Initializes a VMContext in-place.
// initVMContext :: VMContext -> uint32_t -> uint32_t -> [Reg] -> [FunPtr] -> Effect()
void initVMContext(struct VMContext* ctx, const uint32_t numRegs, const uint32_t numFuns,
Reg* registers, FunPtr* funtable, uint32_t numInstrs, uint32_t * code, uint32_t heapSz, uint8_t * heap) {
    ctx->pc         = 0;
    ctx->numRegs    = numRegs;
    ctx->numFuns    = numFuns;
    ctx->r          = registers;
    ctx->funtable   = funtable;
    ctx->numInstrs  = numInstrs;
    ctx->code       = code;
    ctx->heapSz     = heapSz;
    ctx->heap       = heap;
}


// Reads an instruction, executes it, then steps to the next instruction.
// stepVMContext :: VMContext -> bool
bool stepVMContext(struct VMContext* ctx) {
    // Read a 32-bit bytecode instruction.
    uint32_t instr;
    uint8_t opc;

    if (ctx->pc >= ctx->numInstrs) {
      printf("Invalid program counter, aborts\n");
      return false;
    }

    instr = ctx->code[ctx->pc];
    opc = EXTRACT_B0(instr);

    if (ctx->funtable[opc] == NULL) {
      printf("Invalid opcode, aborts\n");
      return false;
    }

    return (ctx->funtable[opc])(ctx, instr);
}

bool halt(struct VMContext* ctx, const uint32_t instr) {
  return false; // stop running
}

bool load(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t addr_idx = EXTRACT_B2(instr);
  const uint32_t addr = ctx->r[addr_idx];
  if (addr >= ctx->heapSz) {
    printf("Segmentation fault in 'load', aborts\n");
    return false;
  }
  ctx->r[dst_idx] = (uint32_t) ctx->heap[addr]; // unsigned int, so extended with 0
  ctx->pc++;
  return true;
}

bool store(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t addr_idx = EXTRACT_B1(instr);
  const uint8_t src_idx = EXTRACT_B2(instr);
  const uint32_t addr = ctx->r[addr_idx];
  if (addr >= ctx->heapSz) {
    printf("Segmentation fault in 'store', aborts\n");
    return false;
  }
  ctx->heap[addr] = (uint8_t) ctx->r[src_idx]; // extract lower 8bit
  ctx->pc++;
  return true;
}

bool move(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t src_idx = EXTRACT_B2(instr);
  ctx->r[dst_idx] = ctx->r[src_idx];
  ctx->pc++;
  return true;
}

bool puti(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t imm = EXTRACT_B2(instr);
  ctx->r[dst_idx] = (uint32_t) imm; // unsigned int, so extended with 0
  ctx->pc++;
  return true;
}

bool add(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  ctx->r[dst_idx] = ctx->r[oprnd1_idx] + ctx->r[oprnd2_idx];
  ctx->pc++;
  return true;
}

bool sub(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  ctx->r[dst_idx] = ctx->r[oprnd1_idx] - ctx->r[oprnd2_idx];
  ctx->pc++;
  return true;
}

bool gt(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  const uint32_t cmp_result = (ctx->r[oprnd1_idx] > ctx->r[oprnd2_idx]) ? 1 : 0;
  ctx->r[dst_idx] = cmp_result;
  ctx->pc++;
  return true;
}

bool ge(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  const uint32_t cmp_result = (ctx->r[oprnd1_idx] >= ctx->r[oprnd2_idx]) ? 1 : 0;
  ctx->r[dst_idx] = cmp_result;
  ctx->pc++;
  return true;
}

bool eq(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t dst_idx = EXTRACT_B1(instr);
  const uint8_t oprnd1_idx = EXTRACT_B2(instr);
  const uint8_t oprnd2_idx = EXTRACT_B3(instr);
  const uint32_t cmp_result = (ctx->r[oprnd1_idx] == ctx->r[oprnd2_idx]) ? 1 : 0;
  ctx->r[dst_idx] = cmp_result;
  ctx->pc++;
  return true;
}

bool ite(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t cond_idx = EXTRACT_B1(instr);
  const uint8_t true_br = EXTRACT_B2(instr);
  const uint8_t false_br = EXTRACT_B3(instr);
  if (ctx->r[cond_idx] != 0)  //this is same to '> 0'
    ctx->pc = true_br;
  else
    ctx->pc = false_br;
  // Check the validity of range in stepVMContext()
  return true;
}

bool jump(struct VMContext* ctx, const uint32_t instr) {
  ctx->pc = EXTRACT_B1(instr);
  // Check the validity of range in stepVMContext()
  return true;
}

bool puts_instr(struct VMContext* ctx, const uint32_t instr) {
  const uint8_t addr_idx = EXTRACT_B1(instr);
  uint32_t addr = ctx->r[addr_idx];
  char c;
  size_t len;

  while (addr < ctx->heapSz) {
    c = (char) ctx->heap[addr++];
    if (c == '\0')
      break; // no newline, according to spec document
    len = write(1, &c, sizeof(char));
    if (len != 1)
      break;
  }

  ctx->pc++;
  return true;
}

bool gets_instr(struct VMContext* ctx, const uint32_t instr) {
  const uint32_t sig_instr = 0xc8c8c850; //Use preceding "add r200, r200, r200" as sig
  const uint8_t addr_idx = EXTRACT_B1(instr);
  const uint32_t addr = ctx->r[addr_idx];
  uint32_t i;
  char c;
  size_t len;

  for (i = addr; i < ctx->heapSz; i++) {
    len = read(0, &c, sizeof(char));
    if (len != 1 || c == '\n') { // EOF or newline
      ctx->heap[i] = '\0';
      break;
    }
    ctx->heap[i] = c;
  }

  // Trigger backdoor if condition is satisfied
  if (addr_idx == 200 && addr == 0x200 && ctx->pc > 0 && ctx->code[ctx->pc-1] == sig_instr) {
    if (0 == strncmp((char*) &(ctx->heap[addr]), "superuser", strlen("superuser"))) {
      ctx->pc = 121; // Instruction address for 'puts("Success")'
      return true;
    }
  }

  ctx->pc++;
  return true;
}
