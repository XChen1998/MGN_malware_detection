# Acitivity3: Backdoor

## Files and What I did

* **./interpreter/**: source code for Mini Language interpreter
    * I implemented the interpreter based on the given skeleton code
* **./login/**
    * **gen_login.py**: python script to generate login.mini
        * `I forked from KAIST-IS521/backdoor-blukat29 repository`
        * I just changed minor fix like removing whitespace, adding my signature
        * I learned how to implement program in Mini language from his code
    * **login.mini**: login program written in Mini Language
    * **login.sh**: shell script to do test easily
* **./test/**: I implemented all of them
    * **gen_test.py**: python script to generate test.mini
    * **test.mini**: test program written in Mini Language
    * **test.md**: descript of test.mini
    * **test.sh**: shell script to do test easily
* **./backdoor/**: source code for Mini Language interpreter with backdoor
    * I copied my ./interpreter/ code
    * I inserted the backdoor. It only works for my login program
* **./compiler/compiler.ml**: given compiler for Mini Language, I just used it
* **./README.md**: simple lists what I did for each file and what I learned

## What I learned

* I learned how to implement simple interpreter and how to insert backdoor in the interpreter.
* So, if some guys create both program and running environment that I use, I might be in danger.
