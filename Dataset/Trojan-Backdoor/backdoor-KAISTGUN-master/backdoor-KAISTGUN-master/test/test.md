# File Description
test.mini file computes a GCD value of two input digits.
To have a complicate operation, I modified the compiler a bit to add my own function.

- Input  : Two digits. Enter a digit one by one.
- Output : A GCD value of two digits.

Running example : 

	Enter two positive integers (1-9) 
	First : 3
	Second : 5
	Answer is : 1


# gcd opcode description
	
	gcd (reg) (reg) (reg)

"gcd" opcode computes a gcd value of memory values located at R2 and R3.
a gcd value will be stored in the memory value located at R1.
 

# Pseudocode

	#include <stdio.h>
    #include <stdlib.h>
	void main(void){

		int R1,R2,R3;
		int gcd = 1;

		printf("Enter two positive integers (1-9)\n");

		printf("First : ");
		scanf("%d",&R1);

		printf("Second : ");
		scanf("%d",&R2);

		if( R1 > 9 || R2 > 9){
			printf("Type only one digit"); 	
			exit(1);
		}

		if( (R1 | R2) == 0){
			printf("Invalid value for gcd\n");
			exit(1);
	    }
	    
	    //Compute GCD
	    for(int i=2; (i<= R1) && (i<= R2) ; i++)
	    {        
			if(R1%i==0 && R2%i==0)
		   		gcd = i;		   	
	    }    

		R3 = gcd;
		printf("Answer is : %d\n",R3); 
	}
  