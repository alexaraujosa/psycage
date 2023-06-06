#include "map_render.h"


//====================================
// Dungeon
//====================================

void print_dungeon(int HEIGHT, int WIDTH, int beginY, int beginX){
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                
                mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(DUNGEON_WALLS));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(DUNGEON_WALLS_VISITED));
            } 
            if(map[y][x] == 0) {

                mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(DUNGEON_FLOOR));
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(DUNGEON_FLOOR_VISITED));
            }
            if (map[y][x] == 2){
                
                mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(DUNGEON_BLOOD));
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(DUNGEON_BLOOD_VISITED));
            }
            if (map[y][x] == 4){
                
                mvaddch(y + beginY, x + beginX, 'D' | COLOR_PAIR(DOOR));
            }

            if(map[y][x] == 7) {
                mvaddch(y + beginY, x + beginX, 'P' | COLOR_PAIR(POTION));
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(DUNGEON_BLOOD_VISITED));
            }
            if(map[y][x] == 6)
                mvaddch(y + beginY, x + beginX, 'i' | COLOR_PAIR(CANDLE));
            if(map[y][x] == 6)
                mvaddch(y + beginY, x + beginX, 'i' | COLOR_PAIR(CANDLE));
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
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                mvaddch(y + beginY,x + beginX, '#' | COLOR_PAIR(SEWERS_WALLS));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(SEWERS_WALLS_VISITED));
            }
            if (map[y][x] == 3) {
                mvaddch(y + beginY, x + beginX, ' ' | COLOR_BLACK);
            }
            if (map[y][x] == 2) {
                mvaddch(y + beginY, x + beginX, '~' | COLOR_PAIR(SEWERS_BLOOD));
        
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '~' | COLOR_PAIR(SEWERS_BLOOD_VISITED));
            }
            if (map[y][x] == 0){
                mvaddch(y + beginY, x + beginX, 'H' | COLOR_PAIR(SEWERS_FLOOR));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, 'H' | COLOR_PAIR(SEWERS_FLOOR_VISITED));
            }
            if (map[y][x] == 4){
                mvaddch(y + beginY, x + beginX, 'D' | COLOR_PAIR(DOOR));
            }
            if(map[y][x] == 7) {
                mvaddch(y + beginY, x + beginX, 'P' | COLOR_PAIR(POTION));
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '~' | COLOR_PAIR(SEWERS_BLOOD_VISITED));
            }
            
            if(map[y][x] == 6)
                mvaddch(y + beginY, x + beginX, 'i' | COLOR_PAIR(CANDLE));
        }
    
        wprintw(g_renderstate->wnd, "\n");
    }

    refresh(); 
}

//==========================
// Asylum
//==========================

void print_asylum(int HEIGHT, int WIDTH, int beginY, int beginX) {
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (map[y][x] == 1) {
                mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(ASYLUM_WALLS));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '#' | COLOR_PAIR(ASYLUM_WALLS_VISITED));
            } 
            if(map[y][x] == 0) {
                mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(ASYLUM_FLOOR));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '.' | COLOR_PAIR(ASYLUM_FLOOR_VISITED));
            }
            if (map[y][x] == 3){
                mvaddch(y + beginY, x + beginX, ' ' | COLOR_BLACK);
            }
            if(map[y][x] == 2){
                mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(ASYLUM_BLOOD));

                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(ASYLUM_BLOOD_VISITED));
            }
            if(map[y][x] == 4){
                mvaddch(y + beginY, x + beginX, 'D' | COLOR_PAIR(DOOR));
            }
            if(map[y][x] == 7) {
                mvaddch(y + beginY, x + beginX, 'P' | COLOR_PAIR(POTION));
                if(visible[y][x] == 2)
                    mvaddch(y + beginY, x + beginX, '*' | COLOR_PAIR(ASYLUM_BLOOD_VISITED));
            }
            if(map[y][x] == 6)
                mvaddch(y + beginY, x + beginX, 'i' | COLOR_PAIR(CANDLE));
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

