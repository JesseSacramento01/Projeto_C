#include <stdio.h>

int main(int argc, char *argv[]){
  FILE *f;
  int c;

  f = fopen(argv[1], "r");
  while (c != EOF)
  {
    c = fgetc(f);
    fputc(c, stdout);
  }
  
  fclose( f );

  return 0;
}