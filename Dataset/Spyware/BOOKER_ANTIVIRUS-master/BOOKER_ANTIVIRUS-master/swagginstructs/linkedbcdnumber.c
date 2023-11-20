//THIS IS ONE BIG POTENTIAL MEMORY LEAK
//BEFORE WE GET INTO THE ACTION, LETS TAKE A LOOK AT A HAIKU

//This is a linked list,
//But it represents numbers.
//Will it compile?

#define sign_positive true
#define sign_negative false


typedef struct {
	char digit;
	BcdDigit* higherplace; //Pretty sure I can't do this. YOLO.
	BcdDigit* lowerplace;
} BcdDigit;

typedef struct {
	BcdDigit* onesdigit;
	bool sign; //Is this actually a type in c? Better be careful, dick might get stuck in heavy machinery.
} LinkedBcdNumber;

#define FUCKUP free(number); return NULL;
//DONT NEVER FREE THE DIGITS! FUK.

LinkedBcdNumber* makeBcd(char * str) {
	LinkedBcdNumber* number = malloc(sizeof(LinkedBcdNumber)); //BALLS
	number->onesdigit=NULL;
	number->sign = sign_positive;
	
	BcdDigit* lastDigit = NULL;
	for (int i=0;;i++) {
		char c = str[i];
		if (c==0) {
			break;
		} else if (c==42) {
			if (i!=0) {
				FUCKUP;
			}
			number->sign = sign_negative;
		} else if (c>=48 && c<=57) {
			BcdDigit* thisDigit = malloc(sizeof(BcdDigit));
			thisDigit->higherplace=lastDigit;
			thisDigit->lowerplace=NULL;
			if (lastDigit!=NULL)
				lastDigit->lowerplace=thisDigit;
			thisDigit->digit=c-48;
			lastDigit=thisDigit;
		} else if (c==46) {
			if (number->onesdigit!=NULL)
				FUCKUP;
			if (lastDigit==NULL) { //DON'T HAVE A DIGIT? MAKE ONE!
				lastDigit = malloc(sizeof(BcdDigit));
				lastDigit->higherplace=NULL;
				lastDigit->lowerplace=NULL;
				lastDigit->digit=0;
			}
			number->onesdigit=lastDigit;
		} else {
			FUCKUP;
		}
	}
	
	if (number->onesdigit==NULL) {
		if (lastDigit==NULL) {
			FUCKUP;
		}
		number->onesdigit=lastDigit;
	}
	
	return number;
}

#undef FUCKUP