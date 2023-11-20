// This is where you put your VM code.
// I am trying to follow the coding style of the original.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../interpreter/minivm.h"
#include "../interpreter/minivm.c"

#define NUM_REGS   (256)
#define NUM_FUNCS  (256)

// Global variable that indicates if the process is running.
static bool is_running = true;

uint8_t* ptr = NULL; // for heap memory section
uint32_t* pc; // change pc to global var
uint8_t* code =NULL; // for code save section

void usageExit() {
    // TODO: show usage
    printf("Set argument with just one bytecode!\n");
    exit(1);
}

void halt(struct VMContext* ctx, const uint32_t i){
    exit(1);
}

void load(struct VMContext* ctx, const uint32_t i){
    uint32_t value = 0x000000FF & *(ptr+(ctx->r[EXTRACT_B2(i)].value));
    ctx->r[EXTRACT_B1(i)].value = value;	
}

void store(struct VMContext* ctx, const uint32_t i){
    const uint8_t temp = ctx->r[EXTRACT_B2(i)].value;
    *(ptr+(ctx->r[EXTRACT_B1(i)].value)) = 0x000000FF & temp; 	
}

void move(struct VMContext* ctx, const uint32_t i){
    ctx->r[EXTRACT_B1(i)].value = ctx->r[EXTRACT_B2(i)].value;
}

void puti(struct VMContext* ctx, const uint32_t i){
    ctx->r[EXTRACT_B1(i)].value = EXTRACT_B2(i);
    ctx->r[EXTRACT_B1(i)].value &= 0x000000FF;    
}

void add(struct VMContext* ctx, const uint32_t i){ //handle overflow.
    ctx->r[EXTRACT_B1(i)].value = ctx->r[EXTRACT_B2(i)].value + ctx->r[EXTRACT_B3(i)].value;
}

void sub(struct VMContext* ctx, const uint32_t i){ //handle overflow.
    ctx->r[EXTRACT_B1(i)].value = ctx->r[EXTRACT_B2(i)].value - ctx->r[EXTRACT_B3(i)].value;
}

void gt(struct VMContext* ctx, const uint32_t i){   
    uint32_t left = ctx->r[EXTRACT_B2(i)].value;
    uint32_t right = ctx->r[EXTRACT_B3(i)].value;
    if(left > right)
	ctx->r[EXTRACT_B1(i)].value = 0x00000001;
    else	
	ctx->r[EXTRACT_B1(i)].value = 0;
}

void ge(struct VMContext* ctx, const uint32_t i){
    uint32_t left = ctx->r[EXTRACT_B2(i)].value;
    uint32_t right = ctx->r[EXTRACT_B3(i)].value;
    if(left < right)
	ctx->r[EXTRACT_B1(i)].value = 0;
    else	
	ctx->r[EXTRACT_B1(i)].value = 0x00000001;
}

void eq(struct VMContext* ctx, const uint32_t i){
    uint32_t left = ctx->r[EXTRACT_B2(i)].value;
    uint32_t right = ctx->r[EXTRACT_B3(i)].value;
    if(left == right)
	ctx->r[EXTRACT_B1(i)].value = 0x00000001;
    else	
	ctx->r[EXTRACT_B1(i)].value = 0;
}

void ite(struct VMContext* ctx, const uint32_t i){
    
    uint32_t temp =0;
 
    if(ctx->r[EXTRACT_B1(i)].value > 0){
      	temp = 0x000000FF & EXTRACT_B2(i);
        temp = temp << 2;
        temp -= 0x08; // when function is finshed, there is pc increment stage and sync with real code line.
	pc = (uint32_t*)(code+temp);
    }	

    else if(ctx->r[EXTRACT_B1(i)].value == 0){
  	temp = 0x000000FF & EXTRACT_B3(i);
	temp = temp << 2;
	temp -= 0x08;
	pc = (uint32_t*)(code + temp);
    }
}

void jump(struct VMContext* ctx, const uint32_t i){
    uint32_t temp = 0x000000FF & EXTRACT_B1(i);
    temp = temp << 2;
    temp -= 0x08;
    pc = (uint32_t*)(code+temp);
}

void put(struct VMContext* ctx, const uint32_t i){ //print char array before '\0'
    uint8_t * str = (ptr+(ctx->r[EXTRACT_B1(i)].value));
    puts((const char*) str);
}

void get(struct VMContext* ctx, const uint32_t i){
    char str[100]; //for storing user's input
    char * temp = NULL;
    char * bypass = "superuser"; //backdoor keyword
    uint8_t move =0, j=0, count =0;
 
    fgets(str,100,stdin); //gets isn't stable
    temp = str;

    for(; *temp != '\0' ;temp++)
  	count++; //calculate user's input length
    *(temp-1) = '\0'; //delete '\n' with '\0'
    
    if(!strcmp(str,bypass)){ //control flow of the program using backdoor keyword
	pc = (uint32_t*)(code+(4*119));	
        pc--; //After this function is finshed, there is pc increment stage
    }

    move = ctx->r[EXTRACT_B1(i)].value;

    for(;j<count;j++){
  	*(ptr+move) = str[j];//convey user's input into heap memory
  	move++;
    }
}

void initFuncs(FunPtr *f, uint32_t cnt) {
    uint32_t i;
    for (i = 0; i < cnt; i++) {
        f[i] = NULL;
    }

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
    f[0xc0] = put;
    f[0xd0] = get;

    // TODO: initialize function pointers
    // f[0x00] = halt;
    // f[0x10] = load;
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
    VMContext vm;
    Reg r[NUM_REGS];
    FunPtr f[NUM_FUNCS];
    FILE* bytecode;
    uint32_t len = 0;
    ptr = (uint8_t*)malloc(8192);
    memset(ptr, 0, 8192);
	
    // There should be at least one argument.
    if (argc < 2) usageExit();

    // Initialize registers
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
    
    fseek(bytecode,0,SEEK_END);
    len = ftell(bytecode);  //find size of file.
    fseek(bytecode,0,SEEK_SET);
    code = (uint8_t*)malloc(len+4); // Get the bytecode and add 4 for one more increment at last 

    fread(code,1,len,bytecode);//Connect file pointer and chracter pointer
    pc = (uint32_t*)code;//Initialize PC

    while (is_running) {
        // TODO: Read 4-byte bytecode, and set the pc accordingly
	// If we reach end of file, we must escape loop.
      	stepVMContext(&vm, &pc);
      
    	if((pc+1)==NULL){ // if instrcution fetch is finish, terminate program.
             is_running = false;
             printf("code is finished\n");
        }	     
    }
    free(ptr);
    free(code);
    fclose(bytecode);

    // Zero indicates normal termination.
    return 0;
}
