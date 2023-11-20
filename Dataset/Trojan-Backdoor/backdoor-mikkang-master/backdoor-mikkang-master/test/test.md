# Test.md

my **test.mini** does a unit test for each opcode.

### Input / Output

you can type any ASCII string as input. then you will get following result.
```
<input><input>P<input><input with first character changed to "P">PPPPPP
```
example
```
input: abc
output: abcabcPabcPbcPPPPPP
```

### pseudocode

```c
    char heap[8192]; //interpreter alloc 8k heap
    // Simple test begin
    // opcode jump
    // expect: test.mini will not be termintated
    goto start;
bad:
    exit(1);
start:
    // opcode puti, gets, puts
    // expect: print string that I typed
    gets(heap + 0xff);
    puts(heap + 0xff);

    // opcode move
    // expect: print string that I typed above again
    void *m = heap + 0xff;
    puts(m);

    // opcode store
    // expect: print "P"
    memcpy(heap, "P", 1);
    puts(heap);

    // opcode load
    // expect: print typed string and typed string with the first character changed to "P"
    puts(heap + 0xff);
    memcpy(heap + 0xff, heap, 1);
    puts(heap + 0xff);

    // opcode ite
    // expect: print "P"
    if (1) puts(heap);
    else goto bad;

    // opcode add, sub
    // expect: print "PP"
    if (0 + 1) puts(heap); else goto bad;
    if (1 - 1) goto bad; else puts(heap);

    // opcode gt
    // expect: print "P"
    if ( !(2 > 3) && (3 > 2) && !(2 > 2) ) puts(heap); else goto bad;

    // opcode ge
    // expect: print "P"
    if ( !(2 >= 3) && (3 >= 2) && (2 >= 2) ) puts(heap); else goto bad;

    // opcode eq
    // expect: print "P"
    if ( !(2 == 3) && !(3 == 2) && (2 == 2) ) puts(heap); else goto bad;

    // opcode halt
    // expect: exit program successfully not print "F"
    exit(1);
    printf("F");
```
