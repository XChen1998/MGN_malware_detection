Activity 3
==========

##### YoungKwang Han

What I made
-----------

1.[./interpreter/](https://github.com/KAIST-IS521/backdoor-juanaevv/tree/master/interpreter)
I just modified interpreter.c to act like VM(Interpreter).I use default skeleton code of interpreter.c.
What I made is various functions corresponding to Mini language's opcode. Also, I made code for handling memory 
area and code section.

2.[./login/login.mini](https://github.com/KAIST-IS521/backdoor-juanaevv/tree/master/login/login.mini)
I wrote code using Mini language to act like "Listing 1" pseudo code.

3.[./test/test.mini](https://github.com/KAIST-IS521/backdoor-juanaevv/tree/master/test/test.mini)
I wrote code using Mini language to demonstrate VM(Interpreter)is running correctly.
Program made from this code execute like pseudo code decribed in test.md.

4.[./test/test.md](https://github.com/KAIST-IS521/backdoor-juanaevv/tree/master/test/test.md)
This file contains pseudo code(in C) of test.mini. Also, it explains how program (made from test.mini)
works and output/input of program.

5.[./backdoor/](https://github.com/KAIST-IS521/backdoor-juanaevv/tree/master/backdoor)
This file contains backdoor.c, which has backdoor code in interpreter.c.
Backdoor.c file creates VM program. However, the VM changes flow of program, which is made from login.mini.
So, if user inputs superuser to answer "User:", VM let user skip password stage.
I modify get function in interpreter.c to achieve this goal.

What I learned
--------------
- During this acitivity, I got to know how interpreter works little bit. Also, I understood concept of backdoor.
  Backdoor concepts can be used in not only system but also software and so on.
- While coding Mini language(assembly language) program, I felt thanks to C language and other high-level
  languages. Nevertheless, I felt that ability to analyze and interpret low-level language is important to 
  security experts.  
