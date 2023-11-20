/************************************************************//**
* @file   main.c                                      		*
* @author Alex Perrakis <csd3826@csd.uoc.gr>  			*
*                                                    		*
* @brief Header function for the needs of cs - 240a project 2017*
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "troy.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
* or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

void InsertAtEmptyList(struct general * current_general, struct DDL_soldier * new_DDL_Soldier)
{
	current_general->soldiers_head = new_DDL_Soldier;
	current_general->soldiers_tail = new_DDL_Soldier;
	new_DDL_Soldier->next = NULL;
	new_DDL_Soldier->prev = NULL;
}

void InsertAtTheStart(struct general * current_general, struct DDL_soldier * new_DDL_Soldier, struct DDL_soldier * current_DDL)
{
	current_DDL->prev = new_DDL_Soldier;
	new_DDL_Soldier->prev = NULL;
	new_DDL_Soldier->next = current_DDL;
	current_general->soldiers_head = new_DDL_Soldier;
}

void InsertAtTheEnd(struct general * current_general, struct DDL_soldier * new_DDL_Soldier, struct DDL_soldier * current_DDL)
{
	current_DDL->next = new_DDL_Soldier;
	new_DDL_Soldier->prev = current_DDL;
	new_DDL_Soldier->next = NULL;
	current_general->soldiers_tail = new_DDL_Soldier;
}

void InsertAtTheMid(struct general * current_general, struct DDL_soldier * new_DDL_Soldier, struct DDL_soldier * current_DDL)
{
	new_DDL_Soldier->next = current_DDL;
	new_DDL_Soldier->prev = current_DDL->prev;
	current_DDL->prev->next = new_DDL_Soldier;
	current_DDL->prev = new_DDL_Soldier;
}

int initialize()
{
	/* Creating the sentinel as a struct soldier in memory */ 
	registration_sentinel = (struct soldier *)malloc(sizeof(struct soldier));

	/* Pointing the registration_list (start of the list) at the sentinel */ 
	registration_list = registration_sentinel;

	/* Initializing the sentinel's values */ 
	registration_sentinel->gid = -1;
	registration_sentinel->sid = -1;
	registration_sentinel->next = NULL;

	/* ------------------------------------------------------------------------- */
	/* Creating the sentinel as a struct general in memory */ 
	general_sentinel = (struct general *)malloc(sizeof(struct general));

	/* Pointing the generals_list (start of the list) at the sentinel */ 
	generals_list = general_sentinel;

	/* Initializing the sentinel's values */ 
	general_sentinel->gid = -1;
	general_sentinel->combats_no = -1;
	general_sentinel->soldiers_head = NULL;
	general_sentinel->soldiers_tail = NULL;
	general_sentinel->next = NULL;

	/* ------------------------------------------------------------------------- */
}

int register_soldier(int sid, int gid)
{
	struct soldier * new_soldier;

	new_soldier = (struct soldier *)malloc(sizeof(struct soldier));
	if (new_soldier == NULL)
	{
		printf("Couldn't malloc a struct soldier.");
		return 0;
	}

	new_soldier->sid = sid;
	new_soldier->gid = gid;
	new_soldier->next = registration_list;

	registration_list = new_soldier;

	print_registration_list();

	return 1;
}

int add_general(int gid)
{
	struct general * new_general;

	new_general = (struct general *)malloc(sizeof(struct general));
	if (new_general == NULL)
	{
		printf("Couldn't malloc a struct general.");
		return 0;
	}

	new_general->gid = gid;
	new_general->soldiers_head = NULL;
	new_general->soldiers_tail = NULL;
	new_general->combats_no = 0;
	new_general->next = generals_list;

	generals_list = new_general;

	print_generals();
	return 1;
}

int print_generals() {

	struct general * current_g;
	struct DDL_soldier * current_DDL;

	printf("\n\tGENERALS:\n");

	current_g = generals_list;
	while (current_g->next != NULL)
	{
		printf("\t%d: ", current_g->gid);

		current_DDL = current_g->soldiers_head;

		while (current_DDL != NULL)
		{
			printf("%d ", current_DDL->sid);
			current_DDL = current_DDL->next;
		}

		current_g = current_g->next;
		printf("\n");

	}
	printf("\nDONE\n");
}

int print_registration_list()
{
	struct soldier * current;

	printf("\n\tRegistration list = ");

	current = registration_list;

	while (current->next != NULL)
	{
		printf("%d:%d", current->sid, current->gid);
		if (current->next->next != NULL)
			printf(", ");

		current = current->next;
	}

	printf("\n\nDONE\n");
}

int distribute()
{
	struct soldier * current_soldier;
	struct general * current_general;
	struct DDL_soldier * current_DDL;
	struct DDL_soldier * new_DDL_Soldier;

	current_soldier = NULL;
	current_general = NULL;
    	current_DDL = NULL;
	new_DDL_Soldier = NULL;

	current_soldier = registration_list;

	while (current_soldier->next != NULL)
	{
		/* *******************************(Search operation)******************************** */
		current_general = generals_list;
		while (current_general->gid != current_soldier->gid)
		{
			current_general = current_general->next;
		}

		new_DDL_Soldier = (struct DDL_soldier *)malloc(sizeof(struct DDL_soldier));

		new_DDL_Soldier->sid = current_soldier->sid;


		/* *******************************(Insert operation)******************************** */

		/* -------------Case that DDL_soldier list of the general is empty----------- */
		if (current_general->soldiers_head == NULL && current_general->soldiers_tail == NULL)
		{
			/* Insert at empty list function */ 
			InsertAtEmptyList(current_general, new_DDL_Soldier);

			current_soldier = current_soldier->next;
			continue;
		}

		/* ----------Case that DDL_soldier list of the general is not empty----------- */
		current_DDL = current_general->soldiers_head;
		while (current_DDL->next != NULL && new_DDL_Soldier->sid > current_DDL->sid)
		{
			current_DDL = current_DDL->next;
		}

		/* Case that DDL_soldier list has only 1 element */ 
		if (current_DDL->prev == NULL && current_DDL->next == NULL)
		{
			if (new_DDL_Soldier->sid > current_DDL->sid)
			{
				InsertAtTheEnd(current_general, new_DDL_Soldier, current_DDL);
			}
			else
				InsertAtTheStart(current_general, new_DDL_Soldier, current_DDL);

			current_soldier = current_soldier->next;
			continue;
		}

		/* Case that DDL_soldier list has more than 1 elements */ 
		if (current_DDL->next == NULL)
		{
			if (new_DDL_Soldier->sid > current_DDL->sid)
			{
				/* Insert at the end function */ 
				InsertAtTheEnd(current_general, new_DDL_Soldier, current_DDL);
			}
			else
				InsertAtTheMid(current_general, new_DDL_Soldier, current_DDL);

			current_soldier = current_soldier->next;
			continue;
		}
		if (current_DDL->prev == NULL)
		{
			InsertAtTheStart(current_general, new_DDL_Soldier, current_DDL);

			current_soldier = current_soldier->next;
			continue;
		}

		/* Case that DDL_soldier must be inserted inside the list */ 
		InsertAtTheMid(current_general, new_DDL_Soldier, current_DDL);
		current_soldier = current_soldier->next;
	}
	print_generals();
}

void deleteAchillesNode(struct general * current_general_ACH, int gid1)
{
	/* Delete the ACH Node from the general list */ 
	/* Case that Achilles is the first general of the list */ 
	struct general * helper;
	current_general_ACH = generals_list;
	if (current_general_ACH->gid == gid1)
	{
		generals_list = current_general_ACH->next;
		free(current_general_ACH);		
		return;
	}

	while (current_general_ACH->next->gid != gid1)
	{
		current_general_ACH = current_general_ACH->next;
	}
	helper = current_general_ACH->next->next;
	free(current_general_ACH->next);
	current_general_ACH->next = helper;	
}

int general_resign(int gid1, int gid2)
{
	struct general * current_general_ACH;
	struct general * current_general_PAT;
	struct general * helper;
	struct DDL_soldier * current_DDL_ACH;
	struct DDL_soldier * current_DDL_PAT;

	current_general_ACH = generals_list;
	while (current_general_ACH->next != NULL && current_general_ACH->gid != gid1)
	{
		current_general_ACH = current_general_ACH->next;
	}

	current_general_PAT = generals_list;
	while (current_general_PAT->next != NULL && current_general_PAT->gid != gid2)
	{
		current_general_PAT = current_general_PAT->next;
	}

	current_DDL_ACH = current_general_ACH->soldiers_head;
	current_DDL_PAT = current_general_PAT->soldiers_head;

	/* Insert At the start */ 
	if (current_DDL_ACH->sid < current_DDL_PAT->sid)
	{
		if (current_general_ACH->soldiers_head->next != NULL)
			current_general_ACH->soldiers_head = current_general_ACH->soldiers_head->next;
		
		current_DDL_ACH->next = current_DDL_PAT;
		current_DDL_PAT->prev = current_DDL_ACH;
		current_general_ACH->soldiers_head->prev = NULL;
		current_general_PAT->soldiers_head = current_general_PAT->soldiers_head->prev;

		current_DDL_ACH = current_general_ACH->soldiers_head;
		current_DDL_PAT = current_general_PAT->soldiers_head;
		
		if (current_general_ACH->soldiers_head == current_general_ACH->soldiers_tail)
		{			
			current_general_ACH->soldiers_head = NULL;
			deleteAchillesNode(current_general_ACH, gid1);
			print_generals();

			return;
		}
	}
	/* -------------------------------------------------------------------------- */
	while (current_general_ACH->soldiers_head != NULL)
	{
		while ((current_DDL_ACH->sid > current_DDL_PAT->sid) && current_DDL_PAT->next != NULL)
		{
			current_DDL_PAT = current_DDL_PAT->next;
		}
		/* Insert at the end */ 
		if (current_DDL_PAT->next == NULL && current_DDL_ACH->sid > current_DDL_PAT->sid)
		{
			current_DDL_PAT->next = current_general_ACH->soldiers_head;
			current_general_ACH->soldiers_head->prev = current_general_PAT->soldiers_tail;
			current_general_PAT->soldiers_tail = current_general_ACH->soldiers_tail;
			break;
		}

		/* Insert at the middle */ 
		if (current_DDL_ACH->next != NULL)
		{
			current_general_ACH->soldiers_head = current_general_ACH->soldiers_head->next;
		}
		else
		{
			current_general_ACH->soldiers_head = NULL;
		}
		current_DDL_ACH->next = current_DDL_PAT;
		current_DDL_ACH->prev = current_DDL_PAT->prev;
		current_DDL_PAT->prev->next = current_DDL_ACH;
		current_DDL_PAT->prev = current_DDL_ACH;

		current_DDL_ACH = current_general_ACH->soldiers_head;
	}
	
	deleteAchillesNode(current_general_ACH, gid1);
	print_generals();
	return 1;
}

int prepare_battle(int gid1, int gid2, int gid3)
{
	struct general * current_general;
	struct DDL_soldier * current_DDL_soldier1start;
	struct DDL_soldier * current_DDL_soldier2start;
	struct DDL_soldier * current_DDL_soldier3start;
	struct DDL_soldier * current_DDL_soldier1end;
	struct DDL_soldier * current_DDL_soldier2end;
	struct DDL_soldier * current_DDL_soldier3end;
	struct c_soldier * current_c_soldier;
	int flag = 0;

	current_DDL_soldier1start = NULL;
	current_DDL_soldier2start = NULL;
	current_DDL_soldier3start = NULL;
	current_DDL_soldier1end = NULL;
	current_DDL_soldier2end = NULL;
	current_DDL_soldier3end = NULL;
	current_c_soldier = NULL;	

	printf("\tCombat soldiers: ");
	/* Searching for the 3 gids in the generals_list and initializing the current_c_soldier<x> pointer  */ 
	current_general = generals_list;
	while (current_general->next != NULL)
	{
		
		if (current_general->gid == gid1)
		{
			current_general->combats_no++;
			current_DDL_soldier1start = current_general->soldiers_head;
			current_DDL_soldier1end = current_general->soldiers_tail;			
		}
		else if (current_general->gid == gid2)
		{
			current_general->combats_no++;
			current_DDL_soldier2start = current_general->soldiers_head;
			current_DDL_soldier2end = current_general->soldiers_tail;			
		}
		else if (current_general->gid == gid3)
		{
			current_general->combats_no++;
			current_DDL_soldier3start = current_general->soldiers_head;
			current_DDL_soldier3end = current_general->soldiers_tail;			
		}

		current_general = current_general->next;
	}
	
	my_combat.soldier_cnt = 0;	
	while (current_DDL_soldier1end->next != current_DDL_soldier1start &&
		current_DDL_soldier2end->next != current_DDL_soldier2start &&
		current_DDL_soldier3end->next != current_DDL_soldier3start)
	{		
		
		if (current_DDL_soldier1end->next != current_DDL_soldier1start)
		{
			my_combat.soldier_cnt++;
			if (!flag)
			{				
				if (my_combat.combat_s == NULL)
				{					
					my_combat.combat_s = malloc(sizeof(struct c_soldier));
					my_combat.combat_s->alive = 1;
					my_combat.combat_s->gid = gid1;
					my_combat.combat_s->sid = current_DDL_soldier1start->sid;
					my_combat.combat_s->next = NULL;
					current_c_soldier = my_combat.combat_s;

				}
				else
				{
					current_c_soldier->next = malloc(sizeof(struct c_soldier));
					current_c_soldier->next->gid = gid1;
					current_c_soldier->next->alive = 1;
					current_c_soldier->next->sid = current_DDL_soldier1start->sid;
					current_c_soldier->next->next = NULL;
					current_c_soldier = current_c_soldier->next;
				}
				current_DDL_soldier1start = current_DDL_soldier1start->next;
			}
			else
			{
				current_c_soldier->next = malloc(sizeof(struct c_soldier));
				current_c_soldier->next->gid = gid1;
				current_c_soldier->next->alive = 1;
				current_c_soldier->next->sid = current_DDL_soldier1end->sid;
				current_c_soldier->next->next = NULL;
				current_c_soldier = current_c_soldier->next;

				current_DDL_soldier1end = current_DDL_soldier1end->prev;
			}

			printf("%d, ", current_c_soldier->sid);

		}
		if (current_DDL_soldier2end->next != current_DDL_soldier2start)
		{
			my_combat.soldier_cnt++;

			if (!flag)
			{
				current_c_soldier->next = malloc(sizeof(struct c_soldier));
				current_c_soldier->next->gid = gid2;
				current_c_soldier->next->alive = 1;
				current_c_soldier->next->sid = current_DDL_soldier2start->sid;
				current_c_soldier->next->next = NULL;
				current_c_soldier = current_c_soldier->next;

				current_DDL_soldier2start = current_DDL_soldier2start->next;
			}
			else
			{
				current_c_soldier->next = malloc(sizeof(struct c_soldier));
				current_c_soldier->next->gid = gid2;
				current_c_soldier->next->alive = 1;
				current_c_soldier->next->sid = current_DDL_soldier2end->sid;
				current_c_soldier->next->next = NULL;
				current_c_soldier = current_c_soldier->next;

				current_DDL_soldier2end = current_DDL_soldier2end->prev;
			}

			printf("%d, ", current_c_soldier->sid);
		}
		if (current_DDL_soldier3end->next != current_DDL_soldier3start)
		{
			my_combat.soldier_cnt++;
			if (!flag)
			{
				current_c_soldier->next = malloc(sizeof(struct c_soldier));
				current_c_soldier->next->gid = gid3;
				current_c_soldier->next->alive = 1;
				current_c_soldier->next->sid = current_DDL_soldier3start->sid;
				current_c_soldier->next->next = NULL;
				current_c_soldier = current_c_soldier->next;

				current_DDL_soldier3start = current_DDL_soldier3start->next;
			}
			else
			{
				current_c_soldier->next = malloc(sizeof(struct c_soldier));
				current_c_soldier->next->gid = gid3;
				current_c_soldier->next->alive = 1;
				current_c_soldier->next->sid = current_DDL_soldier3end->sid;
				current_c_soldier->next->next = NULL;
				current_c_soldier = current_c_soldier->next;

				current_DDL_soldier3end = current_DDL_soldier3end->prev;
			}

			printf("%d, ", current_c_soldier->sid);
			flag = !flag;
		}
	}
	return 1;
}

int battle(int god_favor)
{
	int numOfDead, i, cnt;
	struct c_soldier * current_c_soldier;

	switch (god_favor)
	{
	case 0:
		numOfDead = my_combat.soldier_cnt * 0.40;

		current_c_soldier = my_combat.combat_s;
		for (i = 0; i < numOfDead; i++)
		{
			current_c_soldier->alive = 0;
			current_c_soldier = current_c_soldier->next;
		}
		break;
	case 1:
		cnt = 0;
		current_c_soldier = my_combat.combat_s;
		current_c_soldier->alive = 0;
		current_c_soldier = current_c_soldier->next;
		while (current_c_soldier != NULL)
		{
			cnt++;
			if (cnt == 10)
			{
				current_c_soldier->alive = 0;
				cnt = 0;
			}
			current_c_soldier = current_c_soldier->next;
		}
		break;
	}

	printf("\n\tCombat soldiers:  \n  ");

	current_c_soldier = my_combat.combat_s;
	while (current_c_soldier != NULL)
	{
		printf("%d:%d", current_c_soldier->sid, current_c_soldier->alive);
		if (current_c_soldier->next != NULL)
			printf(", ");
		current_c_soldier = current_c_soldier->next;
	}

	printf("\n\nDONE\n");

	return 1;
}

int trojan_horse()
{
	int i;
	struct general * top5[5];
	struct general * current_general;
	struct general * min;

	current_general = generals_list;
	/* Initializing the array with the first 5 generals. */ 
	for (i = 0; i < 5; i++)
	{
		top5[i] = current_general;
		current_general = current_general->next;
	}

	/* Searching for the minimum combats_no value in the top5[] array */ 
	min = top5[0];
	for (i = 1; i < 5; i++)
	{
		if (top5[i]->combats_no < min->combats_no)
			min = top5[i];
	}

	/* Looping through generals_list */ 
	while (current_general->next != NULL)
	{
		/* Checking if any general has more combats_no than the minimum combats_no general */ 
		if (current_general->combats_no > min->combats_no)
		{
			/* Searching the current minimum in the array */ 
			for (i = 0; i < 5; i++)
			{
				/* Replacing the minimum in the array with the current general */ 
				if (min->combats_no == top5[i]->combats_no)
				{
					top5[i] = current_general;
					break;
				}					
			}

			/* Searching again for the minimum combats_no number in the top5[] array */ 
			min = top5[0];
			for (i = 1; i < 5; i++)
			{
				if (top5[i]->combats_no < min->combats_no)
					min = top5[i];
			}
		}
		current_general = current_general->next;
	}

	printf("\n\tGenerals = ");
	for (i = 0; i < 5; i++)
	{
		printf("%d:%d", top5[i]->gid, top5[i]->combats_no);
		if (i != 4)
			printf(", ");
	}
	printf("\n\nDONE\n");		
}

void removeFromRegistration(int sid)
{
	struct soldier * current_soldier;
	struct soldier * last_current_soldier;
	last_current_soldier = NULL;
	current_soldier = registration_list;
	/* Searching through the registration list for the soldier with the sid we are looking for*/
	while (current_soldier->sid != sid)
	{
		last_current_soldier = current_soldier;
		current_soldier = current_soldier->next;
	}
	/* if current_soldier is the first element of the reg list */
	if (current_soldier == registration_list)
	{
		registration_list = registration_list->next;
		free(current_soldier);
	}
	else
		/* if current_soldier is in the middle of the list */
	{
		last_current_soldier->next = last_current_soldier->next->next;
		free(current_soldier);
	}
	/* current_soldier will never be last because of the sentinel, so no check needed*/
}

int cease_fire()
{
	int mustGoToStart;
	struct general * current_general_1;
	struct general * current_general_2;
	struct general * current_general_3;
	struct c_soldier * current_c_soldier_1;
	struct c_soldier * current_c_soldier_2;
	struct c_soldier * current_c_soldier_3;
	struct DDL_soldier * current_DDL_start_1;
	struct DDL_soldier * current_DDL_start_2;
	struct DDL_soldier * current_DDL_start_3;
	struct DDL_soldier * current_DDL_end_1;
	struct DDL_soldier * current_DDL_end_2;
	struct DDL_soldier * current_DDL_end_3;
	struct DDL_soldier * helper;
	struct soldier * current_soldier;

	current_general_1 = generals_list;
	current_general_2 = generals_list;
	current_general_3 = generals_list;

	current_c_soldier_1 = my_combat.combat_s;
	current_c_soldier_2 = my_combat.combat_s->next;
	current_c_soldier_3 = my_combat.combat_s->next->next;
	
	/* Searching the generals that have the 3 gid from the 3 first soldiers of the combat list */
	while (current_general_1->gid != my_combat.combat_s->gid)
	{
		current_general_1 = current_general_1->next;
	}
	while (current_general_2->gid != my_combat.combat_s->next->gid)
	{
		current_general_2 = current_general_2->next;
	}
	while (current_general_3->gid != my_combat.combat_s->next->next->gid)
	{
		current_general_3 = current_general_3->next;
	}

	current_DDL_start_1 = current_general_1->soldiers_head;
	current_DDL_start_2 = current_general_2->soldiers_head;
	current_DDL_start_3 = current_general_3->soldiers_head;

	current_DDL_end_1 = current_general_1->soldiers_tail;
	current_DDL_end_2 = current_general_2->soldiers_tail;
	current_DDL_end_3 = current_general_3->soldiers_tail;
	
	/* c_soldiers of General 1 */	
	mustGoToStart = 1;
	while (1)
	{		
		if (current_c_soldier_1 == NULL)
			break;
		
		if (current_c_soldier_1->next == NULL)
			break;
		
		if (current_c_soldier_1->next->next == NULL)
			break;		

		if (!current_c_soldier_1->alive)
		{	
			/* Remove Soldier from registration list*/
			removeFromRegistration(current_c_soldier_1->sid);

			if (mustGoToStart)
			{				
				/* If DDL_soldier is the first element */				
				if (current_DDL_start_1->prev == NULL)
				{					
					if (current_DDL_start_1->next != NULL)
					{						
						current_general_1->soldiers_head = current_general_1->soldiers_head->next;
						free(current_general_1->soldiers_head->prev);
						current_general_1->soldiers_head->prev = NULL;
						current_DDL_start_1 = current_general_1->soldiers_head;							
						mustGoToStart = !mustGoToStart;
						current_c_soldier_1 = current_c_soldier_1->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{						
						current_DDL_start_1 = NULL;
					    current_general_1->soldiers_head = NULL;
						current_general_1->soldiers_tail = NULL;
						break;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_start_1->next == NULL)
				{
					current_general_1->soldiers_tail = current_general_1->soldiers_tail->prev;
					free(current_general_1->soldiers_tail->next);
					current_general_1->soldiers_tail->next = NULL;
					break;

				}
				else /* DDL_soldier is in the middle */
				{
					helper = current_DDL_start_1;
					current_DDL_start_1->prev->next = current_DDL_start_1->next;
					current_DDL_start_1->next->prev = current_DDL_start_1->prev;					
					current_DDL_start_1 = current_DDL_start_1->next;
					free(helper);
				}
				current_DDL_start_1 = current_DDL_start_1->next;
			}
			else /* !mustGoToStart */
			{				
				/* If DDL_soldier is the first element */
				if (current_DDL_end_1->prev == NULL)
				{
					if (current_DDL_end_1->next != NULL)
					{
						
						current_general_1->soldiers_head = current_general_1->soldiers_head->next;
						free(current_general_1->soldiers_head->prev);
						current_general_1->soldiers_head->prev = NULL;
						current_DDL_end_1 = current_general_1->soldiers_head;
						mustGoToStart = !mustGoToStart;
						current_c_soldier_1 = current_c_soldier_1->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{
						free(current_DDL_end_1);						
						current_general_1->soldiers_head = NULL;
						current_general_1->soldiers_tail = NULL;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_end_1->next == NULL)
				{
					current_general_1->soldiers_tail = current_general_1->soldiers_tail->prev;
					free(current_general_1->soldiers_tail->next);
					current_general_1->soldiers_tail->next = NULL;					
					current_DDL_end_1 = current_general_1->soldiers_tail;					
					mustGoToStart = !mustGoToStart;
					current_c_soldier_1 = current_c_soldier_1->next->next->next;
					continue;


				}
				else /* DDL_soldier is in the middle */
				{					
					helper = current_DDL_end_1;
					current_DDL_end_1->prev->next = current_DDL_end_1->next;
					current_DDL_end_1->next->prev = current_DDL_end_1->prev;
					current_DDL_end_1 = current_DDL_end_1->prev;
					free(helper);
				}
			}
		}
		
		current_c_soldier_1 = current_c_soldier_1->next->next->next;
		if (mustGoToStart && current_DDL_start_1 != NULL)
			current_DDL_start_1 = current_DDL_start_1->next;
		else if (!mustGoToStart && current_DDL_end_1 != NULL)
			current_DDL_end_1 = current_DDL_end_1->prev;
		mustGoToStart = !mustGoToStart;
		
	}

	/* c_soldiers of General 2*/
	mustGoToStart = 1;
	while (1)
	{
		if (current_c_soldier_2 == NULL)
			break;
		if (current_c_soldier_2->next == NULL)
			break;
		if (current_c_soldier_2->next->next == NULL)
			break;

		if (!current_c_soldier_2->alive)
		{
			/* Remove Soldier from Registration List */
			removeFromRegistration(current_c_soldier_2->sid);
			if (mustGoToStart)
			{
				/* If DDL_soldier is the first element */
				if (current_DDL_start_2->prev == NULL)
				{
					if (current_DDL_start_2->next != NULL)
					{
						current_general_2->soldiers_head = current_general_2->soldiers_head->next;
						free(current_general_2->soldiers_head->prev);
						current_general_2->soldiers_head->prev = NULL;
						current_DDL_start_2 = current_general_2->soldiers_head;
						mustGoToStart = !mustGoToStart;
						current_c_soldier_2 = current_c_soldier_2->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{		
						current_DDL_start_2 = NULL;
						current_general_2->soldiers_head = NULL;
						current_general_2->soldiers_tail = NULL;
						break;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_start_2->next == NULL)
				{
					current_general_2->soldiers_tail = current_general_2->soldiers_tail->prev;
					free(current_general_2->soldiers_tail->next);
					current_general_2->soldiers_tail->next = NULL;
					break;

				}
				else /* DDL_soldier is in the middle */
				{
					helper = current_DDL_start_2;
					current_DDL_start_2->prev->next = current_DDL_start_2->next;
					current_DDL_start_2->next->prev = current_DDL_start_2->prev;
					current_DDL_start_2 = current_DDL_start_2->next;
					free(helper);
				}
				current_DDL_start_2 = current_DDL_start_2->next;
			}
			else /* !mustGoToStart */
			{
				/* If DDL_soldier is the first element */
				if (current_DDL_end_2->prev == NULL)
				{
					if (current_DDL_end_2->next != NULL)
					{
						current_general_2->soldiers_head = current_general_2->soldiers_head->next;
						free(current_general_2->soldiers_head->prev);
						current_general_2->soldiers_head->prev = NULL;
						current_DDL_end_2 = current_general_2->soldiers_head;
						mustGoToStart = !mustGoToStart;
						current_c_soldier_2 = current_c_soldier_2->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{
						free(current_DDL_end_2);
						current_general_2->soldiers_head = NULL;
						current_general_2->soldiers_tail = NULL;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_end_2->next == NULL)
				{
					current_general_2->soldiers_tail = current_general_2->soldiers_tail->prev;
					free(current_general_2->soldiers_tail->next);
					current_general_2->soldiers_tail->next = NULL;					
					current_DDL_end_2 = current_general_2->soldiers_tail;					
					mustGoToStart = !mustGoToStart;
					current_c_soldier_2 = current_c_soldier_2->next->next->next;
					continue;

				}
				else /* DDL_soldier is in the middle */
				{
					helper = current_DDL_end_2;
					current_DDL_end_2->prev->next = current_DDL_end_2->next;
					current_DDL_end_2->next->prev = current_DDL_end_2->prev;
					current_DDL_end_2 = current_DDL_end_2->next;
					free(helper);
				}
			}
		}

		current_c_soldier_2 = current_c_soldier_2->next->next->next;
		if (mustGoToStart && current_DDL_start_2 != NULL)
			current_DDL_start_2 = current_DDL_start_2->next;
		else if (!mustGoToStart && current_DDL_end_2 != NULL)
			current_DDL_end_2 = current_DDL_end_2->prev;
		mustGoToStart = !mustGoToStart;
	}

	/* c_soldiers of General 3 */
	mustGoToStart = 1;
	while (1)
	{
		if (current_c_soldier_3 == NULL)
			break;
		if (current_c_soldier_3->next == NULL)
			break;
		if (current_c_soldier_3->next->next == NULL)
			break;

		if (!current_c_soldier_3->alive)
		{
			/* Remove Soldier from Registration List */
			removeFromRegistration(current_c_soldier_3->sid);
			if (mustGoToStart)
			{
				/* If DDL_soldier is the first element */
				if (current_DDL_start_3->prev == NULL)
				{
					if (current_DDL_start_3->next != NULL)
					{
						current_general_3->soldiers_head = current_general_3->soldiers_head->next;
						free(current_general_3->soldiers_head->prev);
						current_general_3->soldiers_head->prev = NULL;
						current_DDL_start_3 = current_general_3->soldiers_head;
						mustGoToStart = !mustGoToStart;
						current_c_soldier_3 = current_c_soldier_3->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{
						free(current_DDL_start_3);
						current_general_3->soldiers_head = NULL;
						current_general_3->soldiers_tail = NULL;
						break;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_start_3->next == NULL)
				{
					current_general_3->soldiers_tail = current_general_3->soldiers_tail->prev;
					free(current_general_3->soldiers_tail->next);
					current_general_3->soldiers_tail->next = NULL;
					break;

				}
				else /* DDL_soldier is in the middle */
				{
					helper = current_DDL_start_3;
					current_DDL_start_3->prev->next = current_DDL_start_3->next;
					current_DDL_start_3->next->prev = current_DDL_start_3->prev;
					current_DDL_start_3 = current_DDL_start_3->next;
					free(helper);
				}
				current_DDL_start_3 = current_DDL_start_3->next;
			}
			else /* !mustGoToStart */
			{
				/* If DDL_soldier is the first element */
				if (current_DDL_end_3->prev == NULL)
				{
					if (current_DDL_end_3->next != NULL)
					{
						current_general_3->soldiers_head = current_general_3->soldiers_head->next;
						free(current_general_3->soldiers_head->prev);
						current_general_3->soldiers_head->prev = NULL;
						current_DDL_end_3 = current_general_3->soldiers_head;
						mustGoToStart = !mustGoToStart;
						current_c_soldier_3 = current_c_soldier_3->next->next->next;
						continue;
					}
					/* If DDL_soldier is the first and the last element (only 1 element)*/
					else
					{
						free(current_DDL_end_3);
						free(current_general_3->soldiers_head);
						free(current_general_3->soldiers_tail);
						current_general_3->soldiers_head = NULL;
						current_general_3->soldiers_tail = NULL;
					}
				}
				/* If DDL_soldier is the last element */
				else if (current_DDL_end_3->next == NULL)
				{
					current_general_3->soldiers_tail = current_general_3->soldiers_tail->prev;
					free(current_general_3->soldiers_tail->next);
					current_general_3->soldiers_tail->next = NULL;					
					current_DDL_end_3 = current_general_3->soldiers_tail;					
					mustGoToStart = !mustGoToStart;
					current_c_soldier_3 = current_c_soldier_3->next->next->next;
					continue;

				}
				else /* DDL_soldier is in the middle */
				{
					helper = current_DDL_end_3;
					current_DDL_end_3->prev->next = current_DDL_end_3->next;
					current_DDL_end_3->next->prev = current_DDL_end_3->prev;
					current_DDL_end_3 = current_DDL_end_3->next;
					free(helper);
				}
			}
		}

		current_c_soldier_3 = current_c_soldier_3->next->next->next;
		if (mustGoToStart && current_DDL_start_3 != NULL)
			current_DDL_start_3 = current_DDL_start_3->next;
		else if (!mustGoToStart && current_DDL_end_3 != NULL)
			current_DDL_end_3 = current_DDL_end_3->prev;

		mustGoToStart = !mustGoToStart;
	}

	print_generals();
	return 1;
}


int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if ((fin = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch (buff[0]) {

			/* Comment */
		case '#':
			break;

			/* Register soldier
			* R <sid> <gid> */
		case 'R':
		{			
			int sid, gid;
			sscanf(buff, "%c %d %d", &event, &sid, &gid);			
			DPRINT("%c %d %d\n", event, sid, gid);


			if (register_soldier(sid, gid)) {
				DPRINT("R %d %d succeeded\n", sid, gid);
			}
			else {
				fprintf(stderr, "R %d %d failed\n", sid, gid);
			}

			break;
		}

		/* General or king joins the battlefield
		* G <gid> */
		case 'G':
		{
			int gid;

			sscanf(buff, "%c %d", &event, &gid);			
			DPRINT("%c %d\n", event, gid);

			if (add_general(gid)) {
				DPRINT("%c %d succeeded\n", event, gid);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, gid);
			}
			break;
		}

		/* Distribute soldier
		* D */
		case 'D':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (distribute()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			break;
		}

		/* General resigns from battle
		* M <gid1> <gid2> */
		case 'M':
		{
			int gid1, gid2;
			sscanf(buff, "%c %d %d", &event, &gid1, &gid2);
			DPRINT("%c %d %d\n", event, gid1, gid2);

			if (general_resign(gid1, gid2)) {
				DPRINT("%c %d %d succeeded\n", event, gid1, gid2);
			}
			else {
				fprintf(stderr, "%c %d %d failed\n", event, gid1, gid2);
			}
			break;
		}

		/* Prepare for battle
		* P <gid1> <gid2> <gid3> */
		case 'P':
		{
			int gid1, gid2, gid3;
			sscanf(buff, "%c %d %d %d", &event, &gid1, &gid2, &gid3);
			DPRINT("%c %d %d %d\n", event, gid1, gid2, gid3);

			if (prepare_battle(gid1, gid2, gid3)) {
				DPRINT("%c %d %d %d succeeded\n", event, gid1, gid2, gid3);
			}
			else {
				fprintf(stderr, "%c %d %d %d failed\n", event, gid1, gid2, gid3);
			}

			break;
		}

		/* Battle
		* B <god_favor> */
		case 'B':
		{
			int god_favor;
			sscanf(buff, "%c %d\n", &event, &god_favor);
			DPRINT("%c %d\n", event, god_favor);

			if (battle(god_favor)) {
				DPRINT("%c %d succeeded\n", event, god_favor);
			}
			else {
				fprintf(stderr, "%c %d failed\n", event, god_favor);
			}

			break;
		}

		/* Cease fire
		* U */
		case 'U':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (cease_fire()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Trojan Horse
		* T */
		case 'T':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (trojan_horse()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print generals
		* X */
		case 'X':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (print_generals()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			break;
		}

		/* Print registration list
		* Y */
		case 'Y':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if (print_registration_list()) {
				DPRINT("%c succeeded\n", event);
			}
			else {
				fprintf(stderr, "%c failed\n", event);
			}
			break;
		}

		/* Empty line */
		case '\n':
			break;

			/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);

			break;
		}
	}
	
	return (EXIT_SUCCESS);
}
