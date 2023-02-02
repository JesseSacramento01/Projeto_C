#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#include "extra.c"
 

#define MAX_NAME 100 
#define INITIAL_PLAYER_ENERGY 60
#define INITIAL_PLAYER_map 0
#define NO_OBJECT -1
#define NO_TREASURE -1
#define MAX_map_DESCRIPTION 200
#define MAX_mapS 1000
#define NO_WAY -1
#define N_WEAPONS 5
#define N_OF_ENEMIES 3
#define MAXLIN 100

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
    bool alive;
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
/**
 * struc data contém as estruturas Player, Enemy, e Object
*/
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
void initializeMap( int j, int i, int inf );
void initializeEnemy( struct Enemy enemy[] );
void initializeObjects( struct Object objects[] );
char ** getInput( char *argv[] );
void mapsInfo( );
void setDescriptions();


int main( int argc, char *argv[] ){

    
    input = getInput( argv );
    argsC = argc;

    struct Enemy enemy[2];
    struct Player player;
    struct Object objects[3];
        
    initializePlayer( &player );
    mapsInfo();
    initializeObjects( objects );
    initializeEnemy( enemy );
    setDescriptions();

   
    

    struct data d; // vai guardar os dados do player, e do enemy e do object
    d.player = player;

    d.enemy[0] = enemy[0];
    d.enemy[1] = enemy[1]; 

    d.objects[0] = objects[0];
    d.objects[1] = objects[1];
    d.objects[2] = objects[2];
    

    pthread_t thread1, thread2, t_giant;
     
     strcpy(op, "" );

     //criação de três threads, cada um com a sua função
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

void setDescriptions( ){

  int index = 0;
  FILE *f;
  char l[MAXLIN;

  f = fopen("descriptions.txt", "r");
  while( fgets(l, MAXLIN, f) != NULL ){
     
    strcpy(map[index].description, l);
    index++;
  }

  fclose(f);
}


/**
 * recebe como parametro a struct do player, para inicializar os seus membros
*/
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

/**
 * recebe a struct Object como parametro para inicializar os membros
*/
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


void mapsInfo(){

  FILE *file;
    char fileName[] = "file.txt";
    char w[2];
    int n;

    int i = 1;
    int j = 0;

    // Abrir o arquivo em modo de leitura
    file = fopen(fileName, "r");

    // Verificar se o arquivo foi aberto com sucesso
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
    }

    // Ler o arquivo caractere por caractere
    while( fscanf( file, "%s", w ) != EOF ){
        
        initializeMap( i, j, atoi(w) );
        
        
        if ( i == 8){
            printf("%d\n", atoi(w)); 
            j++;
            i = 0;
            printf("j: %d\n", j);
    }  
    i++;
  }

    // Fechar o arquivo
    fclose(file);

    setInfo( ); // para inserir as informações respectivas a as passagens nas salas
    setDescriptions( ); // para inserir as descrições de cada sala no jogo

   
}

void initializeMap ( int i, int j, int inf )
{
        switch (i)
        {
        case 1:
            map[j].north = inf;
            break;
        case 2:
            map[j].south = inf;
            break;
        case 3:
            map[j].east = inf;
            break;
        case 4:
            map[j].west = inf;
            break; 
        case 5:
            map[j].up = inf;
            break;
        case 6:
            map[j].down = inf;
            break;
        case 7:
            map[j].treasure = inf;
            break;
        case 8:
            map[j].object = inf;
            break;
        default:
            break;
        }
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

    enemy[0].alive = true;
    enemy[1].alive = true;

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
    
    if ( d->enemy[e].energy > 0 ){ printf("Energia do Monstro: %d\n", d->enemy[e].energy); }
    else {
        printf( "O Monstro Morreu\n" );
        printf("\n%s", map[CURRENT_PLACE].description);
        printf("\n%s\n", map[CURRENT_PLACE].info);
      
    }
        if ( restLife >= 22 && restLife <= 27){
            printf("O Monstro Fugiu!\n");
            printf("\n%s", map[CURRENT_PLACE].description);
            printf("\n%s\n", map[CURRENT_PLACE].info); 

            if ( e == 0){
                d->enemy[e].map = (d->player.map - d->enemy[e].map) + 3; // manda o monstro 1 para a terceira sala
        }
            else if ( e == 1) { 
                d->enemy[e].map = (d->player.map - d->enemy[e].map) + 10; // manda o monstro 2 para a segunda sala
        }
    }

        if ( d->enemy[e].energy <= 0 ){
            d->enemy[e].alive = false;
        }
 
}

// Todo metodo para fazer o Monstro Atacar

void enemyAttack( struct data *d, int e ){
    while ( d->enemy[e].energy > 0 && d->enemy[e].map == d->player.map && d->player.energy > 0){
    d->player.energy = d->player.energy - d->enemy[e].power;
    printf("energy: %d\n", d->player.energy);
    printf("O Monstro %s deu um golpe\n", d->enemy[e].name);
    sleep(2);
    
        if ( d->player.energy <= 0){
            printf("O Monstro tirou a sua vida!\n");
            printf("Fim de Jogo!");
            strcpy(op, "q");
            exit(0);
        }
    }
}

// Todo metodo para verificar se há uma arma na sala

void getWeapon( struct data d){
    if ( map[CURRENT_PLACE].object != -1 ){
        if ( d->player.map == CURRENT_PLACE + 1; ){
            printf("Pegou o(a) %s que estava caido");
            d->player.object = map[CURRENT_PLACE].object;
        }
    }
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


void * thread1_player( void *ptr )
{ 
    bool diffPlace = false; // variavel que diz se jogadores estão na mesma sala
    struct data *d = (struct data*)ptr; // struct que contem os valores do monstro e do Player
    
    diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;
    
    if ( diffPlace ){
    printf("\n%s", map[CURRENT_PLACE].description);
    printf("\n%s\n", map[CURRENT_PLACE].info); 
    }

    while( strcmp(op, "q") != 0 ){ 
        
        scanf( "%s", op );
        
        diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;
        
        if ( diffPlace || !d->enemy[0].alive || !d->enemy[1].alive){
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
            while ( (!diffPlace && d->enemy[0].alive) ){ // para permanecer na luta enquanto houver a condição satisfeita
                
                //####################################//
                
                
                if ( strcmp(op, "a") == 0 ){
                    if ( d->player.map == d->enemy[0].map && d->enemy[0].alive ){
                        if ( d->enemy[0].alive ){ scanf("%s", op ); }
                        fight( d, 0 ); 
                    }
                    
                    else if ( d->player.map == d->enemy[1].map && d->enemy[1].alive ) {
                        if ( d->enemy[0].alive ){ scanf("%s", op ); }
                        fight( d, 1 ); 
                    }

                diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;  
        
                }
            }
        }
    }
}

void *thread_giant( void *ptr ){
    //Gigante
    struct data *d = (struct data*)ptr;
    int gPos;
     
     while( d->enemy[0].alive ){
        gPos = rand() % 9 + 8;
        d->enemy[1].map = gPos;
        //printf("O Monstro %s esta na sala %d\n", d->enemy[1].name, d->enemy[1].map);
        if ( d->player.map == d->enemy[1].map && d->enemy[1].energy > 0){
        printf("Se encontraram!\n");
        enemyAttack( d, 1 );
        }
        sleep( 4 );
    } 
}

void  *thread_smoke( void *ptr )
{
    // variavel que mostra se o monstro está vivo ou não
    bool smoke_alive = true;
    struct data *d = (struct data*)ptr;
    int pos;
	
    while( d->enemy[0].alive ){

     pos = rand() % 8 + 1; // posição do monstro
     d->enemy[0].map = pos;
     
     //printf("O Monstro %s esta na sala %d\n", d->enemy[0].name, d->enemy[0].map);
     if ( d->player.map == d->enemy[0].map && d->enemy[0].energy > 0 ){
        printf("Se encontraram!\n");
        enemyAttack( d, 0 );
     }
     sleep( 2 );  
  }
}




