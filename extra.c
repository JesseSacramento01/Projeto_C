#include <stdio.h>
#include <string.h>

void getInput( char *argv[] );

int main(int argc, char *argv[]){
  getInput( argv );
}

void getInput( char *argv[] ){
    char superCode[4]; // super codigo que será posta na linha de comando
    strcpy(superCode, argv[1]);
    int startEnergy = argv[2]; // energia inicial
    int startPos = atoi(argv[3]); // posição inicial
    int object = atoi(argv[4]); // objecto que será carregado pelo player
    
    printf("%s %d %d %d", argv[1], argv[2], argv[3], argv[4]);
}
