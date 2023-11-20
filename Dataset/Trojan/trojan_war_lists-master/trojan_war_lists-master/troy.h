/********************************************************//**
 * @file   geo.h                                      	    *
 * @author Antonis Papaioannou <papaioan@csd.uoc.gr>  	    *
 *                                                    	    *
 * @brief Header file for the needs of cs-240a project 2017 *
 ***********************************************************/

#ifndef TROY_H_
#define TROY_H_


struct soldier {
	int sid;
	int gid;
	struct soldier *next;
};

struct DDL_soldier {
	int sid;
	struct DDL_soldier *prev;
	struct DDL_soldier *next;
};

struct general {
	int gid;
	int combats_no;
	struct DDL_soldier *soldiers_head;
	struct DDL_soldier *soldiers_tail;
	struct general *next;
};

struct c_soldier {
	int sid;
	int alive;
	int gid;
	struct c_soldier *next;
};

struct combat {
	int soldier_cnt;
	struct c_soldier *combat_s;
};


/* global variable, pointing at the beginning of the registration list */
struct soldier *registration_list;

/* global variable the sentinel node of the registration list */
struct soldier *registration_sentinel;

/* global variable, pointer to the beginning of the generals list*/
struct general *generals_list;

/* global variable, pointer to the generals list sentinel */
struct general *general_sentinel;

/* global variable holding info about the combat */
struct combat my_combat;

#endif /* TROY_H_ */

