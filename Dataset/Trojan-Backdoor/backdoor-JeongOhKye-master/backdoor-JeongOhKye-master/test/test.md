# My Test Program - Sum-up to N

> This is the test program for validating my interpreter work nice.  
> `test.sh` compiles **_mini_** file into **_minibyte_** then execute

## Fast Run
`./test.sh test.mini`

## Test Description
This test source is for calculating sum up to Number N.   
Because my Program can only show 4 digit number, max number of N is 140 (the sum is 9870)
> **Input --> Number N**    
> **Output --> Sum of up-to number N in decimal number**  

## Pseudo-Code
### Simple Version
```C
void Sumup(){
    int a;
    int sum=0;
    int i=0;
    printf("Sum-up to N (Max:140)\n");
    printf("Input the N: ");
    scanf("%d", &a);
    for(i=0; i<=a; i++){
        sum += i
    }
    printf("Output is: %04d\n", sum);
}
```

### Detail Version
```C
void Sumup(){
    char num[10];
    int i=0;
    int number=0;
    int sum=0;
    char digit[5] = {0, };

    printf("Sum-up to N (Max:140)\n");
    printf("Input the N: ");
    gets(num);
    while(num[i] != 0){
        number *= 10;
        number += (num[i] - 0x30);
        i++;
    }
    i=0;
    while(i<=number){
        sum += i;
        i++;
    }
    i=0;
    while(i<sum){
        digit[3]++;
        if(digit[3] == 10){
            digit[3] = 0;
            digit[2]++;
        }
        if(digit[2] == 10){
            digit[2] = 0;
            digit[1]++;
        }
        if(digit[1] == 10){
            digit[1] = 0;
            digit[0]++;
        }
        i++;
    }
    digit[0] += 48;
    digit[1] += 48;
    digit[2] += 48;
    digit[3] += 48;
    digit[4] = 10;
    printf("Output is: ");
    printf(digit);
}

```
