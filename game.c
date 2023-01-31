#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
 

#define MAX_NAME 100 
#define INITIAL_PLAYER_ENERGY 50
#define INITIAL_PLAYER_CELL 0
#define NO_OBJECT -1
#define NO_TREASURE -1
#define MAX_CELL_DESCRIPTION 200
#define MAX_CELLS 1000
#define NO_WAY -1
#define N_WEAPONS 5
#define N_OF_ENEMIES 3

static int CURRENT_PLACE = 0;
static char op[10];

/**
 * Struct que representa as armas e objectos carregados pelos jogadores
*/
struct Object{
    char name[ MAX_NAME ];
    int power;
};
/**
 * Struct que representa os monstros no jogo
*/
struct Enemy{
    char name[ MAX_NAME ];
    int energy;
    int power;
    int cell;
};
/**
 * Struct que representa O player do jogo
*/
struct Player{
    
    char name[ MAX_NAME ];
    int  energy;
    int  cell;
    int  object;
    int  treasure;
    
};



/**
 * Struct que representa as diferentes salas do jogo
*/
struct Cell{

    int north; // forward
    int south; // back
    int west; // left
    int east; // right
    int up;
    int down;
    char description[ MAX_CELL_DESCRIPTION ];
    int object; 
    int treasure;
    char info[MAX_NAME];

};

struct thread_args{
    struct Player *player;
    struct Cell *map;
};




void initializePlayer( struct Player *player){

    printf( "Bem vindo Player Insira o seu nome:");
    scanf( "%s", player->name);
    printf("Passou");
    
    player->energy = INITIAL_PLAYER_ENERGY; // energy player
    player->cell = INITIAL_PLAYER_CELL;
    player->object = NO_OBJECT;
    player->treasure = NO_TREASURE; 
}

void initializeObjects( struct Object object[]){

    strcpy(object[0].name, "Espada");
    strcpy(object[1].name, "Machado");
    strcpy(object[2].name, "Arco_Flecha");
    
    object[0].power = 5;
    object[1].power = 7;
    object[2].power = 3;
}

void initializeCell (struct Cell cell[])
{
    for (int i = 0; i < 16; i++)
    {
        cell[i].north = NO_WAY;
        cell[i].south = NO_WAY;
        cell[i].west = NO_WAY;
        cell[i].east = NO_WAY;
        cell[i].up = NO_WAY;
        cell[i].down = NO_WAY;
        

        char desc [25];
        sprintf(desc, "Estas na sala %d\n", i);
        strcpy(cell[i].description, desc);
    }

};

// Todo colocar as informaçoes restantes nas outras cells
void setInfo( struct Cell cell[] ){
    //sala 1
    strcpy(cell[0].info, "Há passagens para o Norte, Sul e Leste");
    //sala 2
    strcpy(cell[1].info, "Há passagens para o Norte");
    //sala 3
    strcpy(cell[2].info, "Há passagens para o Sul");
    //sala 4
    strcpy(cell[3].info, "Há passagens para o Norte, Oeste e Leste");
    //sala 5
    strcpy(cell[4].info, "Há passagens para o Norte e Oeste");
    //sala 6
    strcpy(cell[5].info, "Há passagens para o Sul e Oeste");
    //sala 7
    strcpy(cell[6].info, "Há passagens para o Oeste");
    //sala 8
    strcpy(cell[7].info, "Há passagens para o Sul, Leste, Oeste, e para Cima");
    //sala 9
    strcpy(cell[8].info, "Há passagens para o Sul, Leste, Oeste e para Baixo");
    //sala 10
    strcpy(cell[9].info, "Há passagens para o Sul e Oeste");
    //sala 11
    strcpy(cell[10].info, "Há passagens para o Sul e Leste");
    //sala 12
    strcpy(cell[11].info, "Há passagens para o Norte, Leste e Oeste");
    //sala 13
    strcpy(cell[12].info, "Há passagens para o Norte e Oeste");
    //sala 14
    strcpy(cell[13].info, "Há passagens para o Norte e Sul");
    //sala 15
    strcpy(cell[14].info, "Há passagens para o Norte, Sul e Leste");
    //sala 16
    strcpy(cell[15].info, "Há passagens para o Norte");

}

void inicializePath(struct Cell cell[])
{
    //Sala 1
    cell[0].north, cell[0].south, cell[0].east = 3, 2, 4;

    //Sala 2
    cell[1].north = 1;

    //Sala 3
    cell[2].south = 1;

    //Sala 4
    cell[3].north, cell[3].west, cell[3].east = 8, 1, 5;

    //Sala 5
    cell[4].north, cell[4].west = 6, 4;

    //Sala 6
    cell[5].south, cell[5].west = 5, 8;

    //Sala 7
    cell[6].east = 8;
    
    //Sala 8
    cell[7].south, cell[7].east, cell[7].west, cell[7].up = 4, 6, 7, 9;

    //Sala 9
    cell[8].down, cell[8].south, cell[8].east, cell[8].west = 8, 12, 10, 11;

    //Sala 10
    cell[9].south, cell[9].west = 13, 9;

    //Sala 11
    cell[10].south, cell[11].east = 14, 9;

    //Sala 12
    cell[11].north, cell[11].east, cell[11].west = 9, 13, 15;

    //Sala 13
    cell[12].north, cell[12].west = 10, 12;

    //Sala 14
    cell[13].north, cell[13].south = 11, 15;

    //Sala 15
    cell[14].south, cell[14].east, cell[14].north =16, 12, 14;

    //Sala 16
    cell[15].north = 15;
}

void inicializeEnemy (struct Enemy enemy[])
{

    strcpy( enemy[0].name, "Fumaca" );
    strcpy( enemy[1].name, "Gigante" );

    enemy[0].energy = 50;
    enemy[1].energy = 100;

    enemy[0].power = 5;
    enemy[1].power = 15;

    enemy[0].cell = 6;
    enemy[1].cell = 16;
}

void movePlayer()
{

}


void  *thread1_player( void *ptr );
void  *thread_smoke( void *ptr );
void  *thread_giant( void *ptr );

int main(){
    struct thread_args *args = malloc (sizeof (struct thread_args));


    struct Cell map[100];
    struct Enemy enemy[2];
    struct Player player;

    

    
    initializePlayer( &player );
    initializeCell( args->map );
    inicializeEnemy( enemy );
    

    pthread_t thread1, thread2, t_giant;
     
     strcpy(op, "" );

     //criação de duas threads, cada um com a sua função
     pthread_create( &thread1, NULL, thread1_player, args);
     pthread_create( &thread2, NULL, thread_smoke, enemy);
     pthread_create( &t_giant, NULL,  thread_giant, enemy);

     //espera pelo término de cada uma das threads
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     pthread_join( t_giant, NULL);
	 
     //printf("Thread 1 terminou\n");
     //printf("Thread 2 terminou\n");
	 
     return 0;
}






void  *thread1_player( void *ptr )
{ 
    struct Player *player = (struct Player*)ptr;
    //struct Cell *map = (struct Cell*)ptr; 
    
    
    while( strcmp(op, "q") != 0 ){
            
printf("%s", player->name);

     printf("Qual o comando?\n");
     scanf("%s", op );
    }
}

void *thread_giant( void *ptr ){
    //Gigante
    struct Enemy *enemy = (struct Enemy*)ptr;
    int gPos;
     
     while(strcmp(op, "q") != 0){
     gPos = rand() % 9 + 8;
     enemy[1].cell = gPos;
     printf("O Monstro %s esta na sala %d\n", enemy[1].name, enemy[1].cell);
     sleep( 4 );
     }
    

}

void  *thread_smoke( void *ptr )
{
    struct Enemy *enemy = (struct Enemy*)ptr;
    int pos;
	
    while(strcmp(op, "q") != 0){
     // Fumaça
     
     pos = rand() % 8 + 1;
     enemy[0].cell = pos;
     
     
     printf("O Monstro %s esta na sala %d\n", enemy[0].name, enemy[0].cell);
     sleep( 2 );
  }


}
