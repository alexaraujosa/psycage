#include "map_render.h"


//====================================
// Dungeon
//====================================

void print_dungeon(int HEIGHT, int WIDTH){
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                
                mvaddch(y, x, '#' | COLOR_PAIR(DUNGEON_WALLS));
            } 
            if(map[y][x] == 0) {

                mvaddch(y, x, '.' | COLOR_PAIR(DUNGEON_FLOOR));
            }
            if (map[y][x] == 2){
                
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
    
    // Print the map
    for (int j = 0; j < HEIGHT; j++) {
        
        for (int i = 0; i < WIDTH; i++) {
            
            if (map[j][i] == 1) {
                mvaddch(j,i, '#' | COLOR_PAIR(SEWERS_WALLS));
            }
            if (map[j][i] == 3) {
                mvaddch(j, i, ' ' | COLOR_PAIR(NOTHING));
            }
            if (map[j][i] == 2) {
                mvaddch(j, i, '~' | COLOR_PAIR(SEWERS_BLOOD));
            }
            if (map[j][i] == 0){
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
            
            if (map[y][x] == 1) {
                mvaddch(y, x, '#' | COLOR_PAIR(ASYLUM_WALLS));
            } 
            if(map[y][x] == 0) {
                mvaddch(y, x, '.' | COLOR_PAIR(ASYLUM_FLOOR));
            }
            if (map[y][x] == 3){
                mvaddch(y, x, ' ' | COLOR_PAIR(NOTHING));
            }
            if(map[y][x] == 4){
                mvaddch(y, x, '*' | COLOR_PAIR(ASYLUM_BLOOD));
            }
        }
        
        printw("\n");
    }

    refresh();
}

//====================================
// Random Map
//====================================

void print_random_map(int HEIGHT, int WIDTH, int n){

    if (n == 1){
        print_dungeon(HEIGHT, WIDTH);
    }
    if (n == 2){
        print_sewers(HEIGHT, WIDTH);
    }
    if (n == 3){
        print_asylum(HEIGHT, WIDTH);
    }
}

