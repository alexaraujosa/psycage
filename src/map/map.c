#include <stdlib.h>
#include "map.h"
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#define WIDTH 100
#define HEIGHT 30


Coords defaultCoords() {
    Coords coords = (Coords)malloc(sizeof(COORDS));
    if (coords == NULL) return NULL;

    coords->x = 0;
    coords->y = 0;

    return coords;
}

void destroyCoords(Coords coords) {
    free(coords);
}

// void make_borders(int **matrix){

//     // Iterate over the matrix and update the border
//     for (int i = 1; i < HEIGHT - 1; i++) {
//         for (int j = 1; j < WIDTH - 1; j++) {
//             if (matrix[i][j] == 0 && (
//                 matrix[i-1][j] == 1 || 
//                 matrix[i+1][j] == 1 || 
//                 matrix[i][j-1] == 1 || 
//                 matrix[i][j+1] == 1)) {
//                     matrix[i][j] = 1;
//             }
//         }
//     }

//     // Iterate over the matrix again and fill any remaining 0s with 3s
//     for (int i = 0; i < HEIGHT; i++) {
//         for (int j = 0; j < WIDTH; j++) {
//             if (matrix[i][j] == 0) {
//                 matrix[i][j] = 3;
//             }
//         }
//     }
// }

//====================================
// Dungeon
//====================================

#define WALL '#'
#define FLOOR '.'

int dungeon[HEIGHT][WIDTH];

// Initialize the dungeon to all walls
void init_dungeon() {
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            dungeon[y][x] = WALL;
        }
    }
}


void generate_dungeon() {    
    
    // Initialize the dungeon with random floors and walls
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (rand() % 100 < 60) { // % of the floor
                
                dungeon[y][x] = FLOOR;
            } 
            else {
                
                dungeon[y][x] = WALL;
            }
        }
    }

    // Run a cellular automaton to smooth out the dungeon
    for (int i = 0; i < 3; i++) {
        
        int new_dungeon[HEIGHT][WIDTH];
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                int count = 0;
                
                for (int dy = -1; dy <= 1; dy++) {
                    
                    for (int dx = -1; dx <= 1; dx++) {
                        
                        if (dungeon[y+dy][x+dx] == FLOOR) {
                            count++;
                        }
                    }
                }
                if (count >= 5) {
                    new_dungeon[y][x] = FLOOR;
                } 
                else {
                    new_dungeon[y][x] = WALL;
                }
            }
        }
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                dungeon[y][x] = new_dungeon[y][x];
            }
        }
    }
}

void blood_stains_dungeon() {
    
    init_pair(2, COLOR_RED, 7 | A_DIM); // por no render
    attron(COLOR_PAIR(2));

    // Random stains in floor
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (dungeon[y][x] == FLOOR && rand() % 100 < 5){ // % of blood
                mvaddch(y, x, '*' | COLOR_PAIR(2));
            }
        }
    }

    // Run a cellular automaton to smooth out the stains
    for (int i = 0; i < 3; i++) {
        
        int new_dungeon[HEIGHT][WIDTH];
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                int count = 0;
                
                for (int dy = -1; dy <= 1; dy++) {
                    
                    for (int dx = -1; dx <= 1; dx++) {
                        
                        if (dungeon[y+dy][x+dx] == FLOOR) {
                            
                            count++;
                        }
                    }
                }
                
                if (count >= 5) {
                    new_dungeon[y][x] = FLOOR;
                } 
                else {
                    new_dungeon[y][x] = WALL;
                }
            }
        }
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                dungeon[y][x] = new_dungeon[y][x];
            }
        }
    }

    // Redraw the stains with updated positions
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (dungeon[y][x] == FLOOR) {
                
                if (rand() % 100 < 5){ // % of blood
                    mvaddch(y, x, '*' | COLOR_PAIR(2));
                }
            }
        }
    }

    attroff(COLOR_PAIR(2));
}

void print_dungeon(){
    
    initscr(); // a ser retirado
    start_color(); // 
    init_color(6, 400, 400, 400); // a por render (dark grey)
    init_color(7, 10, 10, 10); /// a por render (+- black)
    init_pair(1, 6, 7| A_DIM); // a por render 
    attron(COLOR_PAIR(1));
    

    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (dungeon[y][x] == WALL) {
                
                addch('#' | A_BOLD);
            } 
            else {

                addch('.');
            }
        }
        
        printw("\n");
    }

    attroff(COLOR_PAIR(1)); 
    blood_stains_dungeon();
    refresh();// 
    getch(); //
    endwin(); // a retirar
}

void create_dungeon(){
    
    init_dungeon();
    generate_dungeon();
    print_dungeon();
}

//==========================
// Sewers
//==========================

#define NUM_ROOMS 6
#define ROOM_MIN_WIDTH 4
#define ROOM_MIN_HEIGHT 4
#define CORRIDOR_WIDTH 3
#define MAX_CONNECTIONS 6

typedef struct Room {

    int x;
    int y;
    int width;
    int height;
    struct Room* left;
    struct Room* right;

} Room;

int maze[HEIGHT][WIDTH];

void init_maze(){
    
    // Initialize all cells as walls
    for (int i = 0; i < WIDTH; i++) {
        
        for (int j = 0; j < HEIGHT; j++) {
            maze[j][i] = 1;
        }
    }
}

void print_maze(){


    initscr(); // retirar
    start_color(); //
    init_color(11, 660, 540, 168); // a por render (brown)
    init_pair(9, COLOR_RED, COLOR_BLACK | A_DIM); // por no render
    init_pair(12, 11, COLOR_BLACK); // por no render
    init_pair(10, COLOR_GREEN, COLOR_BLACK | A_DIM); // por no render
    attron(COLOR_PAIR(10));
    
    // Print the maze
    for (int j = 0; j < HEIGHT; j++) {
        
        for (int i = 0; i < WIDTH; i++) {
            
            if (maze[j][i] == 1) {
                addch('#');
            }
            else if (maze[j][i] == 2) {
                mvaddch(j, i, '~' | COLOR_PAIR(9));//
            }
            else {
                mvaddch(j, i, '.' | COLOR_PAIR(12));//
            }
        }
    
        printw("\n");
    }

    attroff(COLOR_PAIR(10)); 
    refresh();// 
    getch(); //
    endwin(); // a retirar
}

Room* create_room_sewers(int x, int y, int width, int height) {
    
    Room* room = (Room*)malloc(sizeof(Room));
    
    if (room == NULL) {
        // Error: couldn't allocate memory
        return NULL;
    }
    
    room->x = x;
    room->y = y;
    room->width = width;
    room->height = height;
    room->left = NULL;
    room->right = NULL;
    
    return room;
}

void split_room_sewers(Room* room) {
    
    int split_horizontal = rand() % 2; // Split either horizontally or vertically
    int max_size = (split_horizontal ? room->height : room->width); // Leave 1 cell border
    
    if (max_size < ROOM_MIN_WIDTH * 2 || max_size < ROOM_MIN_HEIGHT * 2) {
        // Room is too small to split, so return
        return;
    }
    
    int split_position = (rand() % (max_size - ROOM_MIN_WIDTH)) + ROOM_MIN_WIDTH + (split_horizontal ? room->y : room->x);

    // Randomize the order in which the rooms are split
    Room* left_room = NULL;
    Room* right_room = NULL;
    
    if (rand() % 2 == 0) {
        if (split_horizontal) {
            left_room = create_room_sewers(room->x, room->y, room->width, split_position - room->y);
            right_room = create_room_sewers(room->x, split_position + 1, room->width, room->height - (split_position - room->y) - 1);
        } 
        else {
            left_room = create_room_sewers(room->x, room->y, split_position - room->x, room->height);
            right_room = create_room_sewers(split_position + 1, room->y, room->width - (split_position - room->x) - 1, room->height);
        }
    } else {
        if (split_horizontal) {
            right_room = create_room_sewers(room->x, split_position + 1, room->width, room->height - (split_position - room->y) - 1);
            left_room = create_room_sewers(room->x, room->y, room->width, split_position - room->y);
        } 
        else {
            right_room = create_room_sewers(split_position + 1, room->y, room->width - (split_position - room->x) - 1, room->height);
            left_room = create_room_sewers(room->x, room->y, split_position - room->x, room->height);
        }
    }
    
    // Check that the resulting rooms are not too small to split
    if (left_room != NULL && right_room != NULL &&
        ((split_horizontal && (left_room->height < ROOM_MIN_HEIGHT * 2 || right_room->height < ROOM_MIN_HEIGHT * 2))
        || (!split_horizontal && (left_room->width < ROOM_MIN_WIDTH * 2 || right_room->width < ROOM_MIN_WIDTH * 2)))) {
        free(left_room);
        free(right_room);
        return;
    }
    
    if (left_room != NULL && right_room != NULL) {
        create_corridor(left_room, right_room, MAX_CONNECTIONS, CORRIDOR_WIDTH);
        split_room_sewers(left_room);
        split_room_sewers(right_room);
    }
}


void create_corridor(Room* room1, Room* room2, int max_connections, int corridor_width) {
    
    static int num_connections = 0;
    static int prev_x = -1;
    static int prev_y = -1;
    
    // Check if we have reached the maximum number of connections
    if (num_connections >= max_connections) {
        return;
    }
    
    // Randomly select one of the two rooms
    Room* source_room;
    Room* target_room;
    
    if (rand() % 2 == 0) {
        source_room = room1;
        target_room = room2;
    } 
    else {
        source_room = room2;
        target_room = room1;
    }
    
    // Select a random point in the source room
    int source_x = rand() % source_room->width + source_room->x;
    int source_y = rand() % source_room->height + source_room->y;
    
    // Select a random point in the target room
    int target_x = rand() % target_room->width + target_room->x;
    int target_y = rand() % target_room->height + target_room->y;
    
    // If this is not the first corridor, connect it to the previous corridor
    if (prev_x != -1 && prev_y != -1) {
        source_x = prev_x;
        source_y = prev_y;
    }
    
    // Create a corridor between the two points
    int dx = target_x - source_x;
    int dy = target_y - source_y;
    
     if (dx != 0) {
        
        int step = (dx > 0) ? 1 : -1;
        
        for (int x = source_x; x != target_x + step; x += step) {
            
            for (int y = source_y - corridor_width / 2; y <= source_y + corridor_width / 2; y++) {
                
                if (y > 0 && y < HEIGHT-1) {
                    if (y == source_y) {
                        maze[y][x] = 2; // Change middle values to 2
                    } 
                    else {
                        maze[y][x] = 0; // Change rest of the values to 0
                    }
                }
            }
        }

        prev_x = target_x;
        prev_y = source_y;
    }
    
    if (dy != 0) {
        int step = (dy > 0) ? 1 : -1;
    
        for (int y = source_y; y != target_y + step; y += step) {
            
            for (int x = source_x - corridor_width / 2; x <= source_x + corridor_width / 2; x++) {
                
                if (x > 0 && x < WIDTH-1 && maze[y][x] != 2) {
                    
                    if (x > source_x - corridor_width / 2 && x < source_x + corridor_width / 2) {
                        maze[y][x] = 2;
                    } 
                    else {
                        maze[y][x] = 0;
                    }
                }
            }
        }
    
        prev_x = source_x;
        prev_y = target_y;
    }

    // Increment the number of connections
    num_connections++;
}

void create_sewers(){
    
    init_maze();

    Room* root_room = create_room_sewers(0, 0, WIDTH, HEIGHT);

    split_room_sewers(root_room);

    Room* rooms = (Room*)malloc(NUM_ROOMS * sizeof(Room));
    Room* prev_room = NULL;
    
    for (int i = 0; i < NUM_ROOMS; i++) {
        
        int room_width = rand() % (WIDTH - ROOM_MIN_WIDTH) + ROOM_MIN_WIDTH;
        int room_height = rand() % (HEIGHT - ROOM_MIN_HEIGHT) + ROOM_MIN_HEIGHT;
        int room_x = rand() % (WIDTH - room_width);
        int room_y = rand() % (HEIGHT - room_height);
        
        rooms[i] = *create_room_sewers(room_x, room_y, room_width, room_height);

        if (prev_room != NULL) {
            create_corridor(prev_room, &rooms[i], MAX_CONNECTIONS, CORRIDOR_WIDTH);
        }
        
        prev_room = &rooms[i];
    }

    free(rooms);
    
    for (int i = 0; i < HEIGHT; i++) {
        maze[i][0] = 1;
        maze[i][WIDTH-1] = 1;
    }
    
    for (int j = 0; j < WIDTH; j++) {
        maze[0][j] = 1;
        maze[HEIGHT-1][j] = 1;
    }

    make_borders(maze);

    print_maze();
}


//==========================
// Asylum
//==========================

#define MAX_ROOMS 10
#define MIN_SIZE 12
#define MAX_SIZE 17

int asylum[HEIGHT][WIDTH];

// Initialize the "asylum" array with all cells set to 1 (representing walls).
void init_asylum(){
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            asylum[y][x] = 1;
        }
    }
}

// Create a Room struct with random values for its x and y position, width, and height.
Room create_room() {
    
    Room room;

    room.width = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE; // random area
    room.height = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
    room.x = rand() % (WIDTH - room.width - 1) + 1; // random location
    room.y = rand() % (HEIGHT - room.height - 1) + 1;

    return room;
}

// Carve out the specified room in the "asylum" array by setting all cells inside the room to 0.
void carve_room(Room room) {
    
    for (int y = room.y; y < room.y + room.height; y++) {
        
        for (int x = room.x; x < room.x + room.width; x++) {
            asylum[y][x] = 0;
        }
    }
}

// Connect two rooms by carving out a corridor between them.
void connect_rooms(Room source, Room destination) {
    
    int x = source.x + (source.width / 2);
    int y = source.y + (source.height / 2);

    while (x != destination.x + (destination.width / 2)) {
        
        asylum[y][x] = 0;
        x += (x < destination.x + (destination.width / 2)) ? 1 : -1;
    }

    while (y != destination.y + (destination.height / 2)) {
        
        asylum[y][x] = 0;
        y += (y < destination.y + (destination.height / 2)) ? 1 : -1;
    }
}

// Carve out corridors to connect all the rooms in the "rooms" array.
void carve_corridors(Room rooms[], int num_rooms) {
    
    for (int i = 0; i < num_rooms - 1; i++) {
        connect_rooms(rooms[i], rooms[i + 1]);
    }
}

void blood_stains_asylum() {
    
    init_pair(5, COLOR_RED, 8 | A_DIM); // por no render
    attron(COLOR_PAIR(5));

    // Random stains in floor
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (asylum[y][x] == 0 && rand() % 100 < 10){ // % of blood
                mvaddch(y, x, '*' | COLOR_PAIR(5));
            }
        }
    }

    // Run a cellular automaton to smooth out the stains
    for (int i = 0; i < 3; i++) {
        
        int new_asylum[HEIGHT][WIDTH];
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                int count = 0;
                
                for (int dy = -1; dy <= 1; dy++) {
                    
                    for (int dx = -1; dx <= 1; dx++) {
                        
                        if (asylum[y+dy][x+dx] == 0) {
                            
                            count++;
                        }
                    }
                }
                
                if (count >= 5) {
                    new_asylum[y][x] = 0;
                } 
                else {
                    new_asylum[y][x] = 1;
                }
            }
        }
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                asylum[y][x] = new_asylum[y][x];
            }
        }
    }

    // Redraw the stains with updated positions
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (asylum[y][x] == FLOOR) {
                
                if (rand() % 100 < 10){ // % of blood
                    mvaddch(y, x, '*' | COLOR_PAIR(5));
                }
            }
        }
    }

    attroff(COLOR_PAIR(5));
}

// Print the current state of the "asylum" array to the console.
void print_asylum() {
    
    initscr(); // retirar
    start_color(); //
    init_color(8, 1020,1020,1020); // por no render (light grey)
    init_pair(3, COLOR_BLACK, COLOR_WHITE | A_DIM); // por no render
    init_pair(4, COLOR_WHITE, 8 | A_DIM); // por no render
    attron(COLOR_PAIR(3));
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if (asylum[y][x] == 1) {
                addch('#' | A_BOLD);
            } 
            else {
                mvaddch(y, x, '.' | COLOR_PAIR(4));
            }
        }
        
        printw("\n");
    }

    attroff(COLOR_PAIR(3));
    blood_stains_asylum();
    refresh(); //
    getch(); //
    endwin(); // retirar
}

void create_asylum(){
 
    init_asylum();

    Room rooms[MAX_ROOMS];
    int num_rooms = rand() % (MAX_ROOMS - 1) + 2;

    for (int i = 0; i < num_rooms; i++) {
        
        rooms[i] = create_room();
        carve_room(rooms[i]);
    }

    carve_corridors(rooms, num_rooms);
    print_asylum();
}
