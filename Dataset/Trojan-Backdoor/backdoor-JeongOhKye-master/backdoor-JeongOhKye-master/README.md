# Activity3 - Backdoor and Mini-vm


* **compiler** : Mini language compiler which convert mini language into bytecode
* **interpreter** : Source code of mini language interpreter
  * `interpreter.c` : Read the Code and Run each code
  * `minivm.c` : This file have the functions for each opcode. (Most Important file)
  * `minivm.h` : Structures and functions are declared.
* **backdoor** : Source code of mini language interpreter with backdoor  
  * Same content with **interpreter** folder
  * I inserted my backdoor code into `minivm.c`
* **login** : Login program
  * `login.mini` : Login program source code with mini language
* **test** : My test program
  * `test.mini` : My test program source code with mini language
  * `test.md` : The description of my test.mini source code
  * `test.sh` : Just convenience in executing test.mini
* **mycompile** : My Style Compiler
  * For Doing this Activity, I made simple code compiler to make code easily.
  * `login.my` and `test.my` is the original my style code of `login/login.mini` and `test/test.mini`


---
# Feelings
> I can learn the operation method of interpreter. And making the test program is very interesting.
