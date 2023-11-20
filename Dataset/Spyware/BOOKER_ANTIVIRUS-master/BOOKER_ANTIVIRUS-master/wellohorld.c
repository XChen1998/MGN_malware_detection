//void go(char* gooby,int dolan);

#define BIGNUMBER 999999

typedef enum {
	BLACK,
	WHITE,
	ASIAN,
	HISPANIC,
	PACIFIC,
	JEWISH
} Race;

typedef enum {
	HERE,
	THERE,
	BACK,
	NORWAY,
	DEAD
} Location;

typedef struct {
	int monies;
	char* name;
	int debt;
	Race race;
} dicks;

typedef struct {
	char* name;
	int age;
	dicks dick;
	Location location;
} Person;


void increaseMonies(dicks dick, int monies) {
	dick.monies += monies;
}

void decreaseMonies(dicks dick, int monies) {
	dick.monies -= monies;
}

void bringBackTheGirlAndWipeAwayTheDebt(dicks* dick,Race race,Person Elizabeth,Person Lutece,Person Booker)
{
	dick->monies = 0;
	dick->debt = 0;
	Elizabeth.location = BACK;
	Lutece.location = NORWAY;
	Booker.location = DEAD;
}

void go(char* gooby,int dolan) {
	if (gooby[dolan]==0)
		return;
	printf("%c",gooby[dolan]);
	go(gooby,++dolan);
}

void gogogadgetgo(char* gooby) {
	go(gooby,0);
	printf("%c",'\n');
}

int main(int argc, char const *argv[])
{
	gogogadgetgo("HELLO GOOBY!");
	gogogadgetgo("BioShock Infinite (C) 2K Games Inc. 2014");
	gogogadgetgo("Making a Booker");
	Person booker;
	Person elizabeth;
	Person lutece;
	gogogadgetgo("Giving debt.");
	booker.dick.debt = BIGNUMBER;
	elizabeth.location = THERE;
	lutece.location = HERE;
	booker.location = HERE;
	booker.dick.monies = 2;
	lutece.dick.monies = BIGNUMBER;
	lutece.dick.race = WHITE;
	booker.dick.race = WHITE;
	elizabeth.dick.race = JEWISH;
	lutece.age = -4;
	booker.age = 30;
	elizabeth.age = 19;
	increaseMonies(booker.dick, 100);
	bringBackTheGirlAndWipeAwayTheDebt(&booker.dick, WHITE, elizabeth, lutece, booker);
	char mander[420];
	gogogadgetgo("Booker's Debt:");
	gogogadgetgo(itoa(booker.dick.debt, mander,10));
	return 0;
}