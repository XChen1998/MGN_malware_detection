# Backdoor activity

## Changes made to files

- **./interpreter/**
   - interpreter.c : Added code that initializes code segment and heap segment.
     Code segment is filled with bytecode read in from the input file. Also, add
     usage printing.
   - minivm.h : Fix the structure of Reg and VMContext. Reg does not have to
     contain type information, so define uint32_t as Reg. Regarding VMContext,
     added new fields for program counter, code segment and heap segment.
   - minivm.c : Add instruction handling function for each opcode. Also, inline
     dispatch() into stepVMContext() and fix the point where PC is incremented.

- **./compiler/**
   - compiler.ml : Add type checking code and fix a bug where register name with
     trailing characters (e.g. "r100aaa") was not properly rejected. Also, add
     usage printing. These changes were merged into upstream repository in
     commit 62c66c4.

- **./test/**
    - test.mini : Write a test program that parse binary digit string input.
    - test.md : Add detailed description on test program.

- **./login/**
    - login.mini : Write a program that simulates authentication process.
      The program follows specifications given in 0313-backdoor.tex document.

- **./backdoor/**
    - Insert a backdoor that recognizes a specific code pattern from login.mini
      and modifies program counter to bypass authentication when "superuser" is
      given as ID.
    - All other contents are the same to './interpreter/' directory.

## Lessons
  Compiler or hardware backdoor can easily compromise the program, and it's not
  easy to recognize backdoors embedded in such a low-level feature. Also, one
  should not trust that the source code will always operate as it looks.
