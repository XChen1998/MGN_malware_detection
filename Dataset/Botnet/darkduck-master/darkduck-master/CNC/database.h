#ifndef DATABASE_H
#define DATABASE_H

#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <mysql/mysql.h>

MYSQL *mysql_conn;
// static const char query_user_connect[] = "SELECT username FROM users WHERE username = ? AND password = password(?)";
static const char query_user_connect[] = "SELECT username, duration_limit, max_bots, intvl, admin FROM users WHERE username = ? AND password = password(?) LIMIT 1;";

struct user {
 	char username[80];
 	int duration_limit;
 	int max_bots;
 	int intvl;
 	bool admin;
};

void open_database();
void close_database();
void test_error(MYSQL *mysql, int status);
void test_stmt_error(MYSQL_STMT *stmt, int status);

bool connect_user(char *username, char *password, struct user *user);

#endif