# Activity #3 - Backdoor

On the base of mini-VM, I implemented mini-language interpreter and its backdoor. 

* ./interpreter   
    In my VM, the process has two memory areas, 1) TEXT for binary code and 2) HEAP for allocating data. 
    ```
	    char text[ TEXT_SIZE ]; 
	    char heap[ HEAP_SIZE + 1 ]; 
    ```
    And I added program counter for control-flow. 
    ```
    	uint32_t* pc;
    ```

* ./interpreter/interpreter.c  
    Annotate below line, if you want to print debugging message. 
    ```
    	#define VM_DEBUG_MESSAGE
    ```
    
* ./login/login.mini  
    I implementated the pseudocode for login program with mini language. 

* ./test/test.mini  
    I implementated the test program to convert small letters to capital letters. 

* ./test/test.md  
    This explains what test.mini does and how to use it. 

* ./backdoor   
    Backdoor is added on the base code of the interpreter. **My backdoor works in every login with the same semantics.** The algorithm is simple.

    > detect "User: " string in `puts` function.  
    -> set `backdoor_triggering` to `true`   
    -> gets "superuser"   
    -> move the control-flow to the embedded backdoor code.   

    I assumed that the backdoor code is hidden and located in the built-in memory.  
    If the input is "superuser", program counter is set to the address of this backdoor code. 

    ```
    	// print "Success" and halt 
    	char backdoor_text[108] = 
    	    "\x40\x01\x00\x00"  // puti r1, 0
    	    "\x40\x02\x53\x00"  // puti r2, 0x53 
    	    "\x20\x01\x02\x00"  // store r1, r2
    	         ...
    	    "\x00\x00\x00\x00"; // halt          
    ```

---
I have learned the mini-VM and mini-language architecture, and designed how to embed backdoor in HW(VM). I was interesting, although it takes more time than I expected. 