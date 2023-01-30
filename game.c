#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "extra.c"
 

#define MAX_NAME 100 
#define INITIAL_PLAYER_ENERGY 50
#define INITIAL_PLAYER_map 0
#define NO_OBJECT -1
#define NO_TREASURE -1
#define MAX_map_DESCRIPTION 200
#define MAX_mapS 1000
#define NO_WAY -1
#define N_WEAPONS 5
#define N_OF_ENEMIES 3

static int CURRENT_PLACE = 0;
static char op[10];
static char ** input;
static int argsC;

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
    int map;
};
/**
 * Struct que representa O player do jogo
*/
struct Player{
    
    char name[ MAX_NAME ];
    int  energy;
    int  map;
    int  object;
    int  treasure;
    
};

/**
 * Struct que representa as diferentes salas do jogo
*/
struct map{

    int north; // forward
    int south; // back
    int west; // left
    int east; // right
    int up;
    int down;
    char description[ MAX_map_DESCRIPTION ];
    int object; 
    int treasure;
    char info[MAX_NAME];

};

struct data{
    struct Player player;
    struct Enemy enemy[2];
    struct Object objects[3];
};

static struct map map[100];


void *thread1_player( void *ptr );
void *thread_smoke( void *ptr );
void *thread_giant( void *ptr );
void initializePlayer( struct Player *player );
void initializeMap( );
void initializeEnemy( struct Enemy enemy[] );
void initializeObjects( struct Object objects[] );
char ** getInput( char *argv[] );

int main( int argc, char *argv[] ){

    
    input = getInput( argv );
    argsC = argc;

    struct Enemy enemy[2];
    struct Player player;
    struct Object objects[3];
        
    initializePlayer( &player );
    initializeMap( );
    initializeObjects( objects );
    initializeEnemy( enemy );
    

    struct data d; // vai guardar os dados do player, e do enemy
    d.player = player;

    d.enemy[0] = enemy[0];
    d.enemy[1] = enemy[1]; 

    d.objects[0] = objects[0];
    d.objects[1] = objects[1];
    d.objects[2] = objects[2];
    

    pthread_t thread1, thread2, t_giant;
     
     strcpy(op, "" );

     //criação de duas threads, cada um com a sua função
     pthread_create( &thread1, NULL, thread1_player, (void*) &d);
     pthread_create( &thread2, NULL, thread_smoke, (void*) &d);
     pthread_create( &t_giant, NULL,  thread_giant, (void*) &d);
     

     //espera pelo término de cada uma das threads
     pthread_join( thread1, NULL);
     pthread_join( thread2, NULL); 
     pthread_join( t_giant, NULL);
	 
	 
     return 0;
}

void getCommands(){
    printf("ir para Norte(n), Sul(s), Leste(l), Oeste(o), Cima(c), Baixo(b)\n");
    printf("Procurar Tesouro (p)\n");
    printf("Atacar (a)\n");
}


// todo metodo para procurar tesouro e verificar se está na mesma sala que o player


void initializePlayer( struct Player *player ){

    printf( "Bem vindo Player Insira o seu nome:\n");
    scanf( "%s", player->name);
    // super mode
    player->energy = INITIAL_PLAYER_ENERGY;
    player->map = INITIAL_PLAYER_map;
    player->object = NO_OBJECT;
    player->treasure = NO_TREASURE; 
    
    if ( argsC == 5){
    player->energy = atoi(input[1]); // energy player choosen in super mode
    // place choosen in super mode
    player->map = atoi(input[2]);
    // current place tem que receber a posição escolhida em super modo
    CURRENT_PLACE = atoi(input[2]) - 1; 
    // object choosen in super mode
    player->object = atoi(input[3]);  
    }
}

void initializeObjects( struct Object object[]){

    strcpy(object[0].name, "Espada");
    strcpy(object[1].name, "Machado");
    strcpy(object[2].name, "Arco_Flecha");
    
    object[0].power = 5;
    object[1].power = 7;
    object[2].power = 3;
}


// Todo colocar as informaçoes restantes nas outras cells
void setInfo( ){
    //sala 1
    strcpy(map[0].info, "Ha passagens para o Norte, Sul e Leste\n");
    //sala 2
    strcpy(map[1].info, "Ha passagens para o Norte\n");
    //sala 3
    strcpy(map[2].info, "Ha passagens para o Sul\n");
    //sala 4
    strcpy(map[3].info, "Ha passagens para o Norte, Oeste e Leste\n");
    //sala 5
    strcpy(map[4].info, "Ha passagens para o Norte e Oeste\n");
    //sala 6
    strcpy(map[5].info, "Ha passagens para o Sul e Oeste\n");
    //sala 7
    strcpy(map[6].info, "Ha passagens para o Oeste\n");
    //sala 8
    strcpy(map[7].info, "Ha passagens para o Sul, Leste, Oeste, e para Cima\n");
    //sala 9
    strcpy(map[8].info, "Ha passagens para o Sul, Leste, Oeste e para Baixo\n");
    //sala 10
    strcpy(map[9].info, "Ha passagens para o Sul e Oeste\n");
    //sala 11
    strcpy(map[10].info, "Ha passagens para o Sul e Leste\n");
    //sala 12
    strcpy(map[11].info, "Ha passagens para o Norte, Leste e Oeste\n");
    //sala 13
    strcpy(map[12].info, "Ha passagens para o Norte e Oeste\n");
    //sala 14
    strcpy(map[13].info, "Ha passagens para o Norte e Sul\n");
    //sala 15
    strcpy(map[14].info, "Ha passagens para o Norte, Sul e Leste\n");
    //sala 16
    strcpy(map[15].info, "Ha passagens para o Norte\n");

}

void initializePath( )
{
    //Sala 1
    map[0].north = 3;
    map[0].south = 2; 
    map[0].east =  4;

    //Sala 2
    map[1].north = 1;

    //Sala 3
    map[2].south = 1;
    map[2].object = 2;

    //Sala 4
    map[3].north = 8;
    map[3].west = 1;
    map[3].east = 5;

    //Sala 5
    map[4].north = 6;
    map[4].west = 4;

    //Sala 6
    map[5].south = 5;
    map[5].west = 8;

    //Sala 7
    map[6].east = 8;
    
    //Sala 8
    map[7].south = 4;
    map[7].east = 6;
    map[7].west = 7;
    map[7].up = 9;

    //Sala 9
    map[8].down = 8;
    map[8].south = 12;
    map[8].east = 10;
    map[8].west = 11;
    map[8].object = 1;

    //Sala 10
    map[9].south = 13;
    map[9].west = 9;

    //Sala 11
    map[10].south = 14;
    map[10].east = 9;

    //Sala 12
    map[11].north = 9;
    map[11].east = 13;
    map[11].west = 15;
    map[11].object = 0;

    //Sala 13
    map[12].north = 10;
    map[12].west = 12;
    map[12].treasure = 1;

    //Sala 14
    map[13].north = 11;
    map[13].south = 15;

    //Sala 15
    map[14].south = 16;
    map[14].east = 12;
    map[14].north = 14;

    //Sala 16
    map[15].north = 15;
}

void initializeMap ( )
{
    for (int i = 0; i < 16; i++)
    {
        map[i].north = NO_WAY;
        map[i].south = NO_WAY;
        map[i].west = NO_WAY;
        map[i].east = NO_WAY;
        map[i].up = NO_WAY;
        map[i].down = NO_WAY;
        map[i].treasure = NO_TREASURE;
        map[i].object = NO_OBJECT;
        

        char desc [25];
        sprintf(desc, "Estas na sala %d\n", i+1);
        strcpy(map[i].description, desc);
    }

   initializePath( ); // para selecionar o caminho entre as diferentes salas 
   setInfo( ); // para inserir as informações respectivas a cada sala

};

void initializeEnemy (struct Enemy enemy[])
{

    strcpy( enemy[0].name, "Fumaca" );
    strcpy( enemy[1].name, "Gigante" );

    enemy[0].energy = 100;
    enemy[1].energy = 250;

    enemy[0].power = 5;
    enemy[1].power = 15;

    enemy[0].map = 6;
    enemy[1].map = 16;
}


void chooseThePath(  ){
    // usar o x para marcar a posição atual do jogador
        // verifica se a direção escolhida pelo player está disponívels

        if ( strcmp(op, "n") == 0  && map[CURRENT_PLACE].north != -1){
            printf("Porta ao Norte Entrando...\n");
            // Current_Place recebe o valor da sala a seguir
            // Menos 1 por conta do indice, não existe sala 0
            CURRENT_PLACE = map[CURRENT_PLACE].north - 1;
            sleep(2);
            printf("\n%s", map[CURRENT_PLACE].description);
            printf("\n%s\n", map[CURRENT_PLACE].info);
        }
        else if ( strcmp(op, "s") == 0 && map[CURRENT_PLACE].south != -1){
            printf("Porta ao Sul Entrando...\n");
            CURRENT_PLACE = map[CURRENT_PLACE].south - 1;
            sleep(2);
        }
        else if ( strcmp(op, "l") == 0 && map[CURRENT_PLACE].east != -1 ){
            printf("Porta a Leste Entrando...\n");
            CURRENT_PLACE = map[CURRENT_PLACE].east - 1;
            sleep(2);
        }
        else if ( strcmp(op, "o") == 0 && map[CURRENT_PLACE].west != -1){
            printf("Porta a Oeste Entrando...\n");
            CURRENT_PLACE = map[CURRENT_PLACE].west - 1;
            sleep(2);
        }
        else if ( strcmp(op, "c") == 0 && map[CURRENT_PLACE].up != -1){
            printf("Escadas para cima Subindo...\n");
            CURRENT_PLACE = map[CURRENT_PLACE].up - 1;
            sleep(2);
        }
        else if ( strcmp(op, "b") == 0 && map[CURRENT_PLACE].down != -1){
            printf("Escadas para baixo Descendo...\n");
            CURRENT_PLACE = map[CURRENT_PLACE].down - 1;
            sleep(2);
        }
        else{
            printf("Este Caminho esta Bloqueado!\n");
        }
    
}

// Todo metodo para fazer player lutar com o Monstro

void fight( struct data *d, int e ){
    int restLife = d->enemy[e].energy - d->objects[e].power;
    d->enemy[e].energy = restLife;
    printf("%d\n", d->enemy[e].energy);

        if ( restLife >= 22 && restLife <= 27){
            printf("O Monstro Fugiu!\n");
            if ( e == 0){
                d->enemy[e].map = (d->player.map - d->enemy[e].map) + 1; // manda o monstro 1 para a primeira sala
        }
            else if ( e == 1) { 
                d->enemy[e].map = (d->player.map - d->enemy[e].map) + 9; // manda o monstro 2 para a primeira sala
        }
    }
}

// Todo metodo para fazer o Monstro Atacar

void enemyAttack( struct data *d, int e ){
    while ( d->enemy[e].energy > 0 && d->enemy[e].map == d->player.map && d->player.energy > 0){
    d->player.energy = d->player.energy - d->enemy[e].power;
    printf("energy: %d\n", d->player.energy);
    printf("O Monstro %s deu um golpe\n", d->enemy[e].name);
    sleep(2);
    
        if ( d->player.energy == 0){
            printf("O Monstro tirou a sua vida!\n");
            printf("Fim de Jogo!");
            strcpy(op, "q");
            exit(0);
        }
    }
}

// Todo metodo para verificar se há uma arma na sala

void getWeapon(){

}

// Todo metodo para verificar se o tesouro está na sala 

void searchTreasure( struct data *d ){
    if ( strcmp(op, "p") == 0){
        printf("Procurando...\n");
        sleep(2);

     if ( map[CURRENT_PLACE].treasure != -1 ) { // se o tesouro estiver na sala
        printf("Encontrou O tesouro!\n");
        d->player.treasure = 1;
    }
    else{
        printf("\nNao ha nenhum tesouro!\n");
    } 
}
   
}


void  *thread1_player( void *ptr )
{ 
    bool diffPlace = false; // variavel que diz se jogadores estão na mesma sala
    struct data *d = (struct data*)ptr; // struct que contem os valores do monstro e do Player
    
    diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;
    
    if ( diffPlace ){
    printf("\n%s", map[CURRENT_PLACE].description);
    printf("\n%s\n", map[CURRENT_PLACE].info); 
    }

    while( strcmp(op, "q") != 0 ){ 
        
        diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;
        
        scanf( "%s", op );
        if ( strcmp(op, "a") != 0 && diffPlace ){
            chooseThePath( );
            printf("\n%s", map[CURRENT_PLACE].description);
            printf("\n%s\n", map[CURRENT_PLACE].info);
            /* quer dizer que o player já mudou de lugar,
            mais um por conta do indice, Não
            há sala 0*/
            d->player.map = CURRENT_PLACE + 1;
        }

        // caso a opção seja o p, o entra na função para procurar o tesouro
        if ( strcmp(op, "p") == 0 ) {
            searchTreasure( d ); // função que faz a busca pelo tesouro na sala 
        }

        if ( strcmp(op, "a") == 0 ){
            while ( !diffPlace  ){ // para permanecer na luta enquanto houver a condição satisfeita
                
                printf("Atacar (a)\n");
                scanf("%s", op );
                
                if ( strcmp(op, "a") == 0 ){
                    if ( d->player.map == d->enemy[0].map ){ fight( d, 0 ); }
                    
                    else if ( d->player.map == d->enemy[1].map ) { fight( d, 1 ); }
                    
                }
                diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map; 
        
            }
        }
    }
 
}

void *thread_giant( void *ptr ){
    //Gigante
    struct data *d = (struct data*)ptr;
    int gPos;
     
     while(strcmp(op, "q") != 0){
        gPos = rand() % 9 + 8;
        d->enemy[1].map = gPos;
        //printf("O Monstro %s esta na sala %d\n", d->enemy[1].name, d->enemy[1].map);
        if ( d->player.map == d->enemy[1].map ){
        printf("Se encontraram! Ewee\n");
        enemyAttack( d, 1 );
        }
        sleep( 4 );
    } 
}

void  *thread_smoke( void *ptr )
{
    struct data *d = (struct data*)ptr;
    int pos;
	
    while(strcmp(op, "q") != 0){

     pos = rand() % 8 + 1;
     d->enemy[0].map = pos;
     
     //printf("O Monstro %s esta na sala %d\n", d->enemy[0].name, d->enemy[0].map);
     if ( d->player.map == d->enemy[0].map ){
        printf("Se encontraram! Ewee");
        enemyAttack( d, 0 );
     }
     sleep( 2 );  
  }
}




