#ifndef CNC_H
#define CNC_H

#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct cnc {
 	int cnc_fd;
 	bool is_connected;
};

struct cnc new_cnc(int client_fd);
void handle(struct cnc cnc_client);

void send_message(struct cnc cnc_client, char *message);

#endif