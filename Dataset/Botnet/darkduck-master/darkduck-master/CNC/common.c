#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "common.h"

int tokenise_to_argc_argv(char *buffer, int *argc, char *argv[], const int argv_length) { 
  int i = 0;
  for (i = 0 ; i < argv_length ; i++) {
    if ( NULL == (argv[i] = strtok_r( NULL, " ", &buffer)) ) break;
  }
  *argc = i;
  return i; 
}
