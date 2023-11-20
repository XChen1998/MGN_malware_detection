# Activity3 - backdoor

* **backdoor/** : I added heuristic based backdoor code. Codes perform backdoor function by recognizing the user login.
  * [`interpreter.c`](backdoor/interpreter.c) : C program that reads and runs bytecodes.
  * [`minivm.c`](backdoor/minivm.c) : C program that interpret bytecodes and runs backdoor.
  * [`minivm.h`](backdoor/minivm.h) : Header file of minivm.c

* **compiler/**
  * [`compiler.ml`](compiler/compiler.ml) : OCaml source code that reads mini language code and makes bytecodes for mini-vm.
  * [`Makefile`](compiler/Makefile) : Makefile to compile compiler

* **interpreter/**
  * [`interpreter.c`](interpreter/interpreter.c) : C source code that reads and runs bytecodes.
  * [`minivm.c`](interpreter/minivm.c) : C source code that interpret bytecodes.
  * [`minivm.h`](interpreter/minivm.h) : Header file of minivm.c

* **test/**
  * [`test.mini`](test/test.mini) : test code for test interpreter.
  * [`test.md`](test/test.md) : description file for [`test.mini`](test/test.mini)

* **login/**
  * [`login.mini`](login/login.mini) : Mini language code that semantically equivalent with pseudocode for login program.
  * [`init_string_maker.py`](login/init_string_maker.py) : Python script that makes mini language code that initialize hard-coded string.

## What I learned from this activity.
I learned about interpreter level backdoor. Few years ago, I heard about xcode compiler backdoor.(named [XcodeGhost](https://en.wikipedia.org/wiki/XcodeGhost)) And now, I knew how it works.
