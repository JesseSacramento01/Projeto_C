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
#define MAXLIN 200
#define INCREASE_ENERGY 30

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
void setDescriptions( );
void getCommands( );


int main( int argc, char *argv[] ){

    
    
    input = getInput( argv );
    argsC = argc;
    	
    if (argc == 5){
        if ( atoi( input[0] ) != 9119 ){
    	    printf( "Codigo Errado!\nAcesso ao super mode Negado!" );
    	    exit( 0 );
    	 }
    }
    
    
    

    struct Enemy enemy[2];
    struct Player player;
    struct Object objects[3];
        
    initializePlayer( &player );
    mapsInfo();
    initializeObjects( objects );
    initializeEnemy( enemy );
    setDescriptions();

    getCommands( &player );

   
    

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
/**
 * Metodo recebe a struct player para apresentar o nome do player
 * mostra os comandos que podem ser usados no jogo
*/
void getCommands( struct Player *player){
    printf("\n\nBem vindo Player %s esses sao os comandos do Jogo:", player->name);

    printf("\n\nIr para Norte(n), Sul(s), Leste(l), Oeste(o), Cima(c), Baixo(b)\n");
    printf("Procurar Tesouro (p)\n");
    printf("Atacar (a)\n");
    printf("Finalizar o Jogo (q)\n");
}

/**
 * lê de um ficheiro as descrições de cada sala, e coloca no array da estrurura map
*/
void setDescriptions( ){

  int index = 0;
  FILE *f;
  char l[MAXLIN];

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
    strcpy(object[2].name, "Arco&Flecha");
    
    object[0].power = 5;
    object[1].power = 7;
    object[2].power = 3;
}


/**
 * Mostra as informações das possíveis passagens de uma sala para a outra
*/
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
    strcpy(map[6].info, "Ha passagens para o Leste\n");
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

/**
 * Lê de um ficheiro para atualizar as salas inserindo o caminho, tesouro, objecto
*/
void mapsInfo(){

  FILE *file;
    char fileName[] = "mapfile.txt";
    char w[2];

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
            j++;
            i = 0;
    }  
    i++;
  }

    // Fechar o arquivo
    fclose(file);

    setInfo( ); // para inserir as informações respectivas a as passagens nas salas
    setDescriptions( ); // para inserir as descrições de cada sala no jogo

   
}

/**
 * recebe como parametro o i que é a quantidade de membros da structura map, e j que é o indice, e inf que
 * é a informação que vem a aprtir de um ficheiro 
*/
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

/**
 * recebe a struct Enemy para inicializar os seus membros
*/
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
/**
 * Para guaiar o utilizador e mostrar para one pode ir, atualiza também a posição atual
*/
void chooseThePath(  ){
    // usar o x para marcar a posição atual do jogador
        // verifica se a direção escolhida pelo player está disponível
  
			
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




/**
 * recebe data que posui as estruturas do enemy do player e do object e um int que diz qual dos monstros é
 * o que o player está atacando 
 * Método usado pela thread do Player
*/
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


/**
 * recebe data que posui as estruturas do enemy do player e do object e um int que diz qual dos monstros é
 * o que o player está atacando
 * Metodo que é usado pela thread do Monstro
*/
void enemyAttack( struct data *d, int e ){
    while ( d->enemy[e].energy > 0 && d->enemy[e].map == d->player.map && d->player.energy > 0){
    d->player.energy = d->player.energy - d->enemy[e].power;
    printf("Sua Energia: %d\n", d->player.energy);
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

/**
* Metodo que verifica se os monstros foram mortos, e o tespuro foi achado
* Caso se verifique esta condição o jogo termina.
*/
void endGame( struct data *d ){
	if ( !d->enemy[0].alive && !d->enemy[1].alive && d->player.treasure == 1 ){
	 printf("Missão Cumprida!\nrecuperou o tesouro e derrotou os monstros!");
	 printf("O seu reino agradece guerreiro %s", d->player.name);
	 exit(0);
	 
    }
}


/**
 * Método Para apanhar uma arma
 * recebe a struct data como parametro para ter acesso ao player
*/
void getWeapon( struct data *d ){
    char c;
    if ( map[CURRENT_PLACE].object != -1 ){
        if ( d->player.map == CURRENT_PLACE + 1 ){
                if ( d->player.object != -1){
                    printf("Largou o(a) %s e apanhou o(a) %s\n", d->objects[d->player.object].name,
                    d->objects[map[CURRENT_PLACE].object].name);
                    d->player.object = map[CURRENT_PLACE].object;
                }
                else {
                    printf("Pegou o(a) %s que estava caido\n", d->objects[map[CURRENT_PLACE].object].name);
                    d->player.object = map[CURRENT_PLACE].object;
                }
                
            }
            
        }
    }


/**
 * Metodo para procurar pelo tesouro, caso encontre atualiza o membro da struct 
 * do player colocando treasure a 1
*/
void searchTreasure( struct data *d ){
    if ( strcmp(op, "p") == 0){
        printf("Procurando...\n");
        sleep(2);

     if ( map[CURRENT_PLACE].treasure != -1 ) { // se o tesouro estiver na sala
        printf("Encontrou O tesouro!\n");
        d->player.treasure = 1;
    }

     if ( CURRENT_PLACE == 2 || CURRENT_PLACE == 7 || CURRENT_PLACE == 10 ){
    	printf("Encontrou uma pocao ganhou mais 30 de energia\n");
        d->player.energy = d->player.energy + INCREASE_ENERGY;	
     } 
    
     else{
        printf("\nNao ha nenhum tesouro!\n");
    } 
}
   
}

/**
 * Função da thread responsável por receber comandos do player
*/
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
        
        endGame( d ); // verifica se é o fim do jogo
        diffPlace = d->player.map != d->enemy[0].map && d->player.map != d->enemy[1].map;
        
        if ( diffPlace || !d->enemy[0].alive || !d->enemy[1].alive){
            chooseThePath( );
            printf("\n%s", map[CURRENT_PLACE].description);
            printf("\n%s\n", map[CURRENT_PLACE].info);
            /* quer dizer que o player já mudou de lugar,
            mais um por conta do indice, Não
            há sala 0*/
            d->player.map = CURRENT_PLACE + 1;
            getWeapon( d );
        }

        // caso a opção seja o p, o entra na função para procurar o tesouro
        if ( strcmp(op, "p") == 0 ) {
            searchTreasure( d ); // função que faz a busca pelo tesouro na sala 
        }

        if ( strcmp(op, "a") == 0 ){
            while ( (!diffPlace && d->enemy[0].alive) ){ // para permanecer na luta enquanto houver a condição satisfeita
                
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
    exit(0); // finaliza o jogo se o char q for detectado
}
/**
 * Função do Monstro Giant, responsável por movimentar e fazer ações deste monstro
*/
void *thread_giant( void *ptr ){
    //Gigante
    struct data *d = (struct data*)ptr;
    int gPos;
     
     while( d->enemy[0].alive ){
        gPos = rand() % 8 + 9;
        d->enemy[1].map = gPos;
        //printf("O Monstro %s esta na sala %d\n", d->enemy[1].name, d->enemy[1].map);
        if ( d->player.map == d->enemy[1].map && d->enemy[1].energy > 0){
        printf("Se encontraram!\n");
        enemyAttack( d, 1 );
        }
        sleep( 4 );
    } 
}

/**
 * Função do Monstro Smoke, responsável por movimentar e fazer ações deste monstro
*/
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

