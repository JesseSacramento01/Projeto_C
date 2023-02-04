#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

struct Player
{
    char name[ MAX_NAME ];
    int energy;
    int object;
    int map;
    int teature;
};

struct Enemy{
    char name [ MAX_NAME ];
    int energy;
    int power;
    int map;
    bool alive;
};

struct Object
{
    char name[ MAX_NAME ];
    int power;
};

struct Map
{
    int north;
    int south;
    int west;
    int east;
    int up;
    int down;
    char description[ MAX_map_DESCRIPTION ];
    int object;
    int teature;
    char info[ MAX_NAME ];
};

void inicializePlayer( struct Player *pPlayer )
{
    printf( "Bem vindo Player Insira o seu nome:\n" );
    scanf( "%s", pPlayer->name );
    
}

void inicializeObject( struct Object object[] )
{
    strcpy( object[0].name, "Espada" );
    strcpy( object[1].name, "Machado" );
    strcpy( object[2].name, "Arco_Flecha" );

    object[0].power = 5;
    object[1].power = 7;
    object[2].power = 3;
}

void inicializeEnemy( struct Enemy enemy[] )
{
    strcpy( enemy[0].name, "Relampago" );
    strcpy( enemy[1].name, "Trovao" );

    enemy[0].energy = 100;
    enemy[1].energy = 250;

    enemy[0].power = 5;
    enemy[1].power = 15;

    enemy[0].map = 6;
    enemy[1].map = 16;

    enemy[0].alive = true;
    enemy[1].alive = true;
}

void inicializeMap(struct Map map[])
{
    int i;
    for( i = 0; i < 16; i++ )
    {
        map[i].north = NO_WAY;
        map[i].south = NO_WAY;
        map[i].east = NO_WAY;
        map[i].west = NO_WAY;
        map[i].up = NO_WAY;
        map[i].down = NO_WAY;
        map[i].object = NO_WAY;
        map[i].teature = NO_WAY;
    }
}



int main(int argc, char const *argv[])
{
    struct Player *pPlayer;
    inicializePlayer(pPlayer);
    return 0;
}