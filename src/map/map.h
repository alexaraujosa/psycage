/***************************************************************
 * MAP MODULE                                                  *
 *                                                             *
 * This module handles the creation and manipulation of maps.  *
 ***************************************************************/

#ifndef __RL_MAP_H
#define __RL_MAP_H

typedef struct coords {
    int x;
    int y;
} COORDS, *Coords;

typedef struct Room {

    int x;
    int y;
    int width;
    int height;
    struct Room* left;
    struct Room* right;

} Room;

extern int **maze;
extern int **dungeon;
extern int **asylum;

/* FUNCTION SIGNATURES */
Coords defaultCoords();
void destroyCoords(Coords coords);

void create_asylum(int HEIGHT, int WIDTH);
void create_dungeon(int HEIGHT, int WIDTH);
void create_sewers(int HEIGHT, int WIDTH);

void print_sewers(int HEIGHT, int WIDTH);
void print_asylum(int HEIGHT, int WIDTH);
void print_dungeon(int HEIGHT, int WIDTH);


#endif