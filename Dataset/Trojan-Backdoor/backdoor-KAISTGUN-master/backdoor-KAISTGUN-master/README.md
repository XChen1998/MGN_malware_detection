# IS-521 Activity 3

This repository is for my activity 3.

Two programs are executable.

- test.mini  - Refer to test.md
- login.mini - Works as a login program given by pseudocode.
             backdoor can be added by interpreter.

# Usage

I added my own script for convenience.
	
	make 				         // compile
	python execute.py [TESTCASE] // run


Running example

	python execute.py test
	python execute.py login
	python execute.py backdoor

# File history

Added
- backdoor/interpreter.c : Add backdoor routine.
- execute.py 			 : A script for executing my program.
- test/test.mini 		 : My own program written in mini language.
- test/test.md 		     : Simple description for my program.
- login/login.mini  	 : Login program written in mini language.

Modified
- compiler/compiler.ml   : Add a gcd opcode.
- interpreter/interpreter.c : Add function for opcodes.
- Makefile      : Add a set of rule to compile codes.

# What I learned

I was able to learn how compiler works.

Without OS, I should manage all of memory spaces (Heap, Code, Reg etc.). It is very cumbersome.

I spent a lot of time in engineering to do this project,so I learned how I can reduce time for debugging, making bytecode efficiently. 
