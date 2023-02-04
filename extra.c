#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char ** getInput( char *argv[] ){

  static char * input[4][4];

  input[0][0] = (char*) argv[1]; // supercode
  input[0][1] = (char*) argv[2]; // start Energy
  input[0][2] = (char*) argv[3]; // start position
  input[0][3] = (char*) argv[4]; // object
  
  return *input;
}
