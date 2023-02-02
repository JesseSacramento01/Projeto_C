#include <stdio.h>
#include <stdlib.h>
#define MAXWORD 20

int main(int argc, char *argv[]){
  FILE *f;
  char w[MAXWORD];
  int i = 0;

  f = fopen("file.txt", "r");
  while( fscanf( f, "%s", w ) != EOF ){
    //fprintf( stdout, "%s\n", w );
    printf("%d\n", atoi(w));
    //printf("%d", i);

  }
  fclose(f);

  return 0;
}