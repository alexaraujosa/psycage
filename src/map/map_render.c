#include "map_render.h"


//====================================
// Dungeon
//====================================

void print_dungeon(int HEIGHT, int WIDTH, int beginY, int beginX){
    
    for (int y = 0; y < HEIGHT - beginY; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                
                mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(DUNGEON_WALLS));
            } 
            if(map[y][x] == 0) {

                mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(DUNGEON_FLOOR));
            }
            if (map[y][x] == 2){
                
                mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(DUNGEON_BLOOD));
            }
        }
        
        wprintw(g_renderstate->wnd, "\n");
    }
    
    refresh();
}

//==========================
// Sewers
//==========================

void print_sewers(int HEIGHT, int WIDTH, int beginY, int beginX){
    
    // Print the map
    for (int j = 0; j < HEIGHT - beginY; j++) {
        
        for (int i = 0; i < WIDTH; i++) {
            
            if (map[j][i] == 1) {
                mvaddch(j + beginY,i + beginX, '#' | COLOR_PAIR(SEWERS_WALLS));
            }
            if (map[j][i] == 3) {
                mvaddch(j + beginY, i + beginX, ' ' | COLOR_BLACK);
            }
            if (map[j][i] == 2) {
                mvaddch(j + beginY, i + beginX, '~' | COLOR_PAIR(SEWERS_BLOOD));
            }
            if (map[j][i] == 0){
                mvaddch(j + beginY, i + beginX, 'H' | COLOR_PAIR(SEWERS_FLOOR));
            }
        }
    
        wprintw(g_renderstate->wnd, "\n");
    }

    refresh(); 
}

//==========================
// Asylum
//==========================

void print_asylum(int HEIGHT, int WIDTH, int beginY, int beginX) {
    
    for (int y = 0; y < HEIGHT - beginY; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(ASYLUM_WALLS));
            } 
            if(map[y][x] == 0) {
                mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(ASYLUM_FLOOR));
            }
            if (map[y][x] == 3){
                mvaddch(y + beginY, x + beginX, ' ' | COLOR_BLACK);
            }
            if(map[y][x] == 4){
                mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(ASYLUM_BLOOD));
            }
        }
        
        wprintw(g_renderstate->wnd, "\n");
    }

    refresh();
}

//====================================
// Random Map
//====================================

void print_random_map(int HEIGHT, int WIDTH, int n, int beginY, int beginX){

    if (n == 1){
        print_dungeon(HEIGHT, WIDTH, beginY, beginX);
        drawGameInterface();
    }
    if (n == 2){
        print_sewers(HEIGHT, WIDTH, beginY, beginX);
        drawGameInterface();
    }
    if (n == 3){
        print_asylum(HEIGHT, WIDTH, beginY, beginX);
        drawGameInterface();
    }
}

