# Description on test.mini

* Input and output of the program
This program takes in number 'n' encoded in binary digit string (e.g. "110" for
n=6). Then the program prints character `A` 'n' times, followed by a newline 
character. Note that program does not verify whether the input string has a 
valid binary digit string format.

* Pseudocode of program

```C
int i, n;
char * buf;

i = n = 0;
buf = malloc(8192);

gets(buf);
while (buf[i] != '\0') {
  n *= 2;
  n += buf[i] - '0'
}

for(i=0; i <n; i++)
  puts("A");

```

* Example of execution

```
$ ocaml str.cma compiler/compiler.ml  ./test/test.mini ./test/test.bin
$ ./interpreter/build/interpreter ./test/test.bin 
110
AAAAAA
```
