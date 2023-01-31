#include <stdio.h>
#include <string.h>
#include <stdlib.h>







char ** getInput( char *argv[] ){

  //printf("%s %s %s %s", argv[1], argv[2], argv[3], argv[4]);

  
  static char * input[4][4];

  input[0][0] = (char*) argv[1]; // supercode
  input[0][1] = (char*) argv[2]; // start Energy
  input[0][2] = (char*) argv[3]; // start position
  input[0][3] = (char*) argv[4]; // object

 
  //printf("%s %s %s %s", input[0][0], input[0][1], input[0][2], input[0][3]);
  
  return *input;
}
