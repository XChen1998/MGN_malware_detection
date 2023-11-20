/*
 *	some_cmd.c
 *
 */

#include "../include/some_hds.h"

void dothings(char *instructions){
	char *token;
	char *command;
	char *temp;
	//token = strsep(&instructions, delim);
	while((token=strsep(&instructions,delim)) != NULL){
		temp = (char *) malloc(strlen(token));
		strcpy(temp, token);
		if(strncmp(temp, "run", strlen("run"))==0){
			command = (char *) malloc(strlen(temp)-4);
			strcpy(command, temp+4);
			printf("command: %s token: %s\n", command, temp);
			system(command);
			free(command);
		}else if(strncmp(temp, "changesleep", strlen("changesleep"))==0){
			command = (char *) malloc(strlen(temp)-strlen("changesleep")-1);
			strcpy(command, temp+strlen("changesleep")+1);
			printf("%d\n", atoi(command));
			sleeptime=atoi(command);
			free(command);
		}
		free(temp);
		//token = strsep(&instructions, delim);
	}
}
