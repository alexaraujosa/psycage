/***************************************************************
 * MAP MODULE                                                  *
 *                                                             *
 * This module handles the creation and manipulation of maps.  *
 ***************************************************************/

#ifndef __RL_MAP_H
#define __RL_MAP_H

#include "common.h"
#include "map_common.h"
#include "../entity/ai/pathfind.h"

typedef struct Room {

    int x;
    int y;
    int width;
    int height;
    struct Room* left;
    struct Room* right;

} Room;

extern int **map;

/* FUNCTION SIGNATURES */
void create_sewers();
void create_dungeon();
void create_asylum();
void create_ai_test_map();
void prepare_ai_test_map();
void print_ai_test_map();
void randomize_atm_points();

void create_asylum(int HEIGHT, int WIDTH, int beginY, int beginX);
void create_dungeon(int HEIGHT, int WIDTH, int beginY, int beginX);
void create_sewers(int HEIGHT, int WIDTH, int beginY, int beginX);

void print_sewers(int HEIGHT, int WIDTH, int beginY, int beginX);
void print_asylum(int HEIGHT, int WIDTH, int beginY, int beginX);
void print_dungeon(int HEIGHT, int WIDTH, int beginY, int beginX);


#endif