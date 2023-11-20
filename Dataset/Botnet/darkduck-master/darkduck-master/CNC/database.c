#include <mysql.h>
#include <stdbool.h>

#include "database.h"
#include "common.h"

void open_database() {

  mysql_conn = mysql_init(NULL);
 
  if (mysql_conn == NULL) {
#ifdef DEBUG
    printf("%s", mysql_error(mysql_conn));
#endif
    perror("Darkduck : Error during MYSQL init ! \n");
    exit(errno);
  }

  if (mysql_real_connect(mysql_conn, DB_ADDR, DB_USER, DB_PASS, DB_DATABASE, 0, NULL, 0) == NULL) {
#ifdef DEBUG
    printf("%s", mysql_error(mysql_conn));
#endif
    perror("Darkduck : Error during MYSQL connection ! \n");
    exit(errno);
  }  
}

void close_database() {
  mysql_close(mysql_conn);
}

bool connect_user(char *username, char *password, struct user *user){


    MYSQL_STMT *stmt = NULL;
    stmt = mysql_stmt_init(mysql_conn);
    if (stmt == NULL) {
      fprintf(stderr, "ERROR:mysql_stmt_init() failed.\n");
      exit(1);
    }

    int status = mysql_stmt_prepare(stmt, query_user_connect, strlen(query_user_connect));
    test_stmt_error(stmt, status);


    MYSQL_BIND input_bind[2];
    memset(input_bind, 0, sizeof(input_bind));

    input_bind[0].buffer_type = MYSQL_TYPE_STRING;
    input_bind[0].buffer = username;
    input_bind[0].buffer_length = strlen(username);
    input_bind[0].length = 0;
    input_bind[0].is_null = 0;

    input_bind[1].buffer_type = MYSQL_TYPE_STRING;
    input_bind[1].buffer = password;
    input_bind[1].buffer_length = strlen(password);
    input_bind[1].length = 0;
    input_bind[1].is_null = 0;


    status = mysql_stmt_bind_param(stmt, input_bind);
    test_stmt_error(stmt, status);

    status = mysql_stmt_execute(stmt);
    test_stmt_error(stmt, status);

    /* Fetch result set meta information */
    MYSQL_RES* prepare_meta_result = mysql_stmt_result_metadata(stmt);
    if (!prepare_meta_result)
    {
        fprintf(stderr,
                " mysql_stmt_result_metadata(), \
                returned no meta information\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
        exit(1);
    }

    /* Get total columns in the query */
    int column_count= mysql_num_fields(prepare_meta_result);
    if (column_count != 5) /* validate column count */
    {
        fprintf(stderr, " invalid column count returned by MySQL\n");
        exit(1);
    }

    MYSQL_BIND result_bind[5];
    memset(result_bind, 0, sizeof(result_bind));

    result_bind[0].buffer_type = MYSQL_TYPE_STRING;
    result_bind[0].buffer = (*user).username;
    result_bind[0].buffer_length = 50;
    result_bind[0].length = 0;
    result_bind[0].is_null = 0;

    result_bind[1].buffer_type = MYSQL_TYPE_LONG;
    result_bind[1].buffer = (char*) &(*user).duration_limit;
    result_bind[1].buffer_length = sizeof((*user).duration_limit);
    result_bind[1].length = 0;
    result_bind[1].is_null = 0;

    result_bind[2].buffer_type = MYSQL_TYPE_LONG;
    result_bind[2].buffer = (char*) &(*user).max_bots;
    result_bind[2].buffer_length = sizeof((*user).max_bots);
    result_bind[2].length = 0;
    result_bind[2].is_null = 0;

    result_bind[3].buffer_type = MYSQL_TYPE_LONG;
    result_bind[3].buffer = (char*) &(*user).intvl;
    result_bind[3].buffer_length = sizeof((*user).intvl);
    result_bind[3].length = 0;
    result_bind[3].is_null = 0;

    result_bind[4].buffer_type = MYSQL_TYPE_LONG;
    result_bind[4].buffer = (char*) &(*user).admin;
    result_bind[4].buffer_length = sizeof((*user).admin);
    result_bind[4].length = 0;
    result_bind[4].is_null = 0;

    status = mysql_stmt_bind_result(stmt, result_bind);
    test_stmt_error(stmt, status);

    if(!mysql_stmt_fetch(stmt)) {
#ifdef DEBUG
  printf("Darkduck : Somebody is connected \n"); 
  printf("\t username : %s \n", (*user).username); 
  printf("\t duration_limit : %d \n", (*user).duration_limit);
  printf("\t max_bots : %d \n", (*user).max_bots); 
  printf("\t intvl : %d \n", (*user).intvl); 
  printf("\t admin : %d \n", (*user).admin);
#endif
      return true;

    }
  
  return false;
}

void test_error(MYSQL *mysql, int status) {
  if (status) {
    fprintf(stderr, "Darkduck : %s (errno: %d) \n", mysql_error(mysql), mysql_errno(mysql));
    exit(errno);
  }
}

void test_stmt_error(MYSQL_STMT *stmt, int status) {
  if (status) {
    fprintf(stderr, "Error: %s (errno: %d) \n", mysql_stmt_error(stmt), mysql_stmt_errno(stmt));
    exit(errno);
  }
}