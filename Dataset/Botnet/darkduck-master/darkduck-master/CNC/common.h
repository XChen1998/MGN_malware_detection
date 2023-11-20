#ifndef COMMON_H
#define COMMON_H

#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct attack {
	bool attack;
 	char ip[20];
 	int port;
};

static const int	MAX_CLIENT 		= 10;
static const int	BUFFER_SIZE		= 2000;

static const char DB_ADDR[] 		= "localhost";
static const char DB_USER[] 		= "root";
static const char DB_PASS[] 		= "root";
static const char DB_DATABASE[] 	= "darkduck";
static const int 	CNC_PORT		= 1080;
static const int 	BOT_PORT		= 9811;

int tokenise_to_argc_argv(char *buffer, int *argc, char *argv[], const int argv_length);

static const char HOLO[]			= "\e[5;49;31m Dark duck : ";
static const char SHELL[]			= "\e[1;49;94m Dark duck shell > \e[5;49;31m$\e[0m ";
static const char CONNECTED[]		= "\e[5;49;31m Dark duck : hi admin  \e[0m\n";
static const char NOT_CONNECTED[]	= "\e[5;49;31m Dark duck : Who are you ? ! \e[0m\n";

static const char WELCOME[]			= "\n"
"############################################# \n"
"############__---~~~~~|~~~~~--__############# \n"
"########.-~~          |          ~~-.######## \n"
"#####.-~     .-~~~~-. |              ~-.##### \n"
"####/       {  o     }|                 \#### \n"
"###/        /       / |                  \### \n"
"##|        `--r'   {  | ,___.-',          |## \n"
"##|          /      ~-|         ',        |## \n"
"##|---------{---------|----------'--------|## \n"
"##|          \        |         /         |## \n"
"##|           \       |        /          |## \n"
"###\         ~ ~~~~~~~|~~~~~~~~~ ~       /### \n"
"####\       ~ ~ ~ ~ ~ | ~ ~ ~ ~ ~ ~     /#### \n"
"#####`-_     ~ ~ ~ ~ ~|~ ~ ~ ~ ~ ~    _-'#### \n"
"########`-__    ~ ~ ~ | ~ ~ ~ ~   __-'####### \n"
"############~~---_____|_____---~~############ \n"
"############################################# \n \n";
#endif