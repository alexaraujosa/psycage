#include "map_render.h"


//====================================
// Dungeon
//====================================

void print_dungeon(int HEIGHT, int WIDTH){
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (dungeon[y][x] == 1) {
                
                mvaddch(y, x, '#' | COLOR_PAIR(DUNGEON_WALLS));
            } 
            if(dungeon[y][x] == 0) {

                mvaddch(y, x, '.' | COLOR_PAIR(DUNGEON_FLOOR));
            }
            if (dungeon[y][x] == 3){
                
                mvaddch(y, x, '*' | COLOR_PAIR(DUNGEON_BLOOD));
            }
        }
        
        printw("\n");
    }
    
    refresh();
}

//==========================
// Sewers
//==========================

void print_sewers(int HEIGHT, int WIDTH){
    
    // Print the maze
    for (int j = 0; j < HEIGHT; j++) {
        
        for (int i = 0; i < WIDTH; i++) {
            
            if (maze[j][i] == 1) {
                mvaddch(j,i, '#' | COLOR_PAIR(SEWERS_WALLS));
            }
            if (maze[j][i] == 3) {
                mvaddch(j, i, ' ' | COLOR_PAIR(NOTHING));
            }
            if (maze[j][i] == 2) {
                mvaddch(j, i, '~' | COLOR_PAIR(SEWERS_BLOOD));
            }
            if (maze[j][i] == 0){
                mvaddch(j, i, 'H' | COLOR_PAIR(SEWERS_FLOOR));
            }
        }
    
        printw("\n");
    }

    refresh(); 
}

//==========================
// Asylum
//==========================

void print_asylum(int HEIGHT, int WIDTH) {
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (asylum[y][x] == 1) {
                mvaddch(y, x, '#' | COLOR_PAIR(ASYLUM_WALLS));
            } 
            if(asylum[y][x] == 0) {
                mvaddch(y, x, '.' | COLOR_PAIR(ASYLUM_FLOOR));
            }
            if (asylum[y][x] == 3){
                mvaddch(y, x, ' ' | COLOR_PAIR(NOTHING));
            }
            if(asylum[y][x] == 4){
                mvaddch(y, x, '*' | COLOR_PAIR(ASYLUM_BLOOD));
            }
        }
        
        printw("\n");
    }

    refresh();
}