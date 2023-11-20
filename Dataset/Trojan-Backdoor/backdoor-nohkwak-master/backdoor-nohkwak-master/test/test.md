# Test program

This program converts small letters to capital letters. 

If you enter some string, then this program will check whether each character is small letter or not. If small letter, the letter will be changed into capital letter. 

> Input : user's input (Max size of string = 50)
> Output : user's input which is converted to the capital letter 

Pseudocode is like following. 

``` C
void main(void){
    int i; 
    char *buf = malloc( /*some amount */ );

    printf( "user input : " );
    gets( buf );
    
    for ( i=0 ; i<  /* some amount */ ; i++ ){
        if ( 0x61 <= buf[i] && buf[i] <= 0x7a ) {
            buf[i] -= 20; 
        }
        else if (buf[i] == 0) {
            break; 
        }
    }

    puts( buf ); 

    exit(0);
}
```

Annotation of the code is like follwing. 

```
// start of test program 

// load text data and come back 
    jump 30
    
1:
// puts "User input: "
    puti r1, 0
    puts r1

// gets 
    puti r1, 55         // heap address : 55 ~ 104
    gets r1

// for ( i=0 ; i<  /* some amount */ ; i++ )
    puti r2, 0          // iterator i=0 
    puti r3, 1          // 1 for i++
    puti r4, 50         // max string size ( 50 in this case)
    puti r10, 105       // make the end of string (50 + 55) as NULL 
    store r10, r2
10: sub  r5, r4, r2     // checking condition 
    ite  r5, 12, 26

// load buf[i]
    add  r6, r1, r2
    load r7, r6

// go next if buf[i] > 0, end loof if not
    ite  r7, 15, 26 

15: 
// go next if buf[i] >= 'a', check next buf[i+1] if not  
    puti r10, 0x60
    gt   r8, r7, r10
    ite  r8, 18, 24

18: 
// go next if 'z' >= buf[i], check next buf[i+1] if not
    puti r10, 0x7b
    gt   r9, r10, r7
    ite  r9, 21, 24

21: 
// buf[i] -= 0x20, this means calpitalizing. 
    puti r10, 0x20
    sub  r7, r7, r10 
    store r6, r7

24: 
// increase iterator for checking next buf[i+1]
    add  r2, r2, r3
    jump 10
    
26:
// puts the capitalized string 
    puti r1, 55
    puts r1

// exit(0)
    halt

30:
// loading "User input: " string to heap from the address 0 
    puti r1, 0
    puti r2, 0x55
    store r1, r2
    puti r1, 1
    puti r2, 0x73
    store r1, r2
    puti r1, 2
    puti r2, 0x65
    store r1, r2
    puti r1, 3
    puti r2, 0x72
    store r1, r2
    puti r1, 4
    puti r2, 0x20
    store r1, r2
    puti r1, 5
    puti r2, 0x69
    store r1, r2
    puti r1, 6
    puti r2, 0x6E
    store r1, r2
    puti r1, 7
    puti r2, 0x70
    store r1, r2
    puti r1, 8
    puti r2, 0x75
    store r1, r2
    puti r1, 9
    puti r2, 0x74
    store r1, r2
    puti r1, 10
    puti r2, 0x3a
    store r1, r2
    puti r1, 11
    puti r2, 0x20
    store r1, r2
    puti r1, 12
    puti r2, 0
    store r1, r2

// go back to the main logic 
    jump 1
```