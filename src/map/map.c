#include "common.h"
#include "map.h"

int **map;

void make_borders(int **matrix, int HEIGHT, int WIDTH) {
    
    // Iterate over the matrix and update the border
    for (int i = 0; i < HEIGHT; i++) {
        
        for (int j = 0; j < WIDTH; j++) {      
            
            if(i == 0 && (matrix[i+1][j] == 1 || matrix[i+1][j] == 3)) matrix[i][j] = 3;
            if(i == HEIGHT-1 && (matrix[i-1][j] == 1 || matrix[i-1][j] == 3)) matrix[i][j] = 3;
            if(j == WIDTH-1 && (matrix[i][j-1] == 1 || matrix[i][j-1] == 3)) matrix[i][j] = 3;
            if(j == 0 && (matrix[i][j+1] == 1 || matrix[i][j+1] == 3)) matrix[i][j] = 3;

            if(i > 0 && i < HEIGHT-1 && j > 0 && j < WIDTH-1){
                
                if(matrix[i][j] == 1 && 
                    (matrix[i-1][j] == 1 || matrix[i-1][j] == 3) && 
                    (matrix[i+1][j] == 1 || matrix[i+1][j] == 3) && 
                    (matrix[i][j+1] == 1 || matrix[i][j+1] == 3) && 
                    (matrix[i][j-1] == 1 || matrix[i][j-1] == 3)){
                        matrix[i][j] = 3;
                }
            }
        }
    }
}

//====================================
// Dungeon
//====================================
#pragma region Dungeon

// Initialize the map to all walls
void init_dungeon(int HEIGHT, int WIDTH) {

    // Allocate space for matrix "map"
    map = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        map[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            map[y][x] = 1;
        }
    }
}


void generate_dungeon(int HEIGHT, int WIDTH) {    
    
    // Initialize the map with random floors and walls
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (rand() % 100 < 60) { // % of the floor
                
                map[y][x] = 0;
            } 
            else {
                
                map[y][x] = 1;
            }
        }
    }

    // Run a cellular automaton to smooth out the map
    for (int i = 0; i < 3; i++) {
        
        int new_dungeon[HEIGHT][WIDTH];
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                int count = 0;
                
                for (int dy = -1; dy <= 1; dy++) {
                    
                    for (int dx = -1; dx <= 1; dx++) {
                        
                        if (map[y+dy][x+dx] == 0) {
                            count++;
                        }
                    }
                }
                if (count >= 5) {
                    new_dungeon[y][x] = 0;
                } 
                else {
                    new_dungeon[y][x] = 1;
                }
            }
        }
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                map[y][x] = new_dungeon[y][x];
            }
        }
    }
}

void blood_stains_dungeon(int HEIGHT, int WIDTH) {

    // Random stains in floor
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (map[y][x] == 0 && rand() % 100 < 5){ // % of blood
                map[y][x] = 2;
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
                        
                        if (map[y+dy][x+dx] == 0) {
                            
                            count++;
                        }
                    }
                }
                
                if (count >= 5) {
                    new_dungeon[y][x] = 0;
                } 
                else {
                    new_dungeon[y][x] = 1;
                }
            }
        }
        
        for (int y = 1; y < HEIGHT - 1; y++) {
            
            for (int x = 1; x < WIDTH - 1; x++) {
                
                map[y][x] = new_dungeon[y][x];
            }
        }
    }

    // Redraw the stains with updated positions
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (map[y][x] == 0) {
                
                if (rand() % 100 < 5){ // % of blood
                    map[y][x] = 2;
                }
            }
        }
    }
}

void create_dungeon( int HEIGHT, int WIDTH, int beginY, int beginX){
    
    init_dungeon(HEIGHT, WIDTH);
    generate_dungeon(HEIGHT, WIDTH);
    blood_stains_dungeon(HEIGHT, WIDTH);
    print_dungeon(HEIGHT, WIDTH, beginY, beginX);
}

//==========================
// Sewers
//==========================
#pragma region Sewers

#define NUM_ROOMS 10
#define ROOM_MIN_WIDTH 7
#define ROOM_MIN_HEIGHT 7
#define CORRIDOR_WIDTH 3

void init_maze(int HEIGHT, int WIDTH){
    
    // Allocate space for the matrix "map"
    map = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        map[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    
    // Initialize all cells as walls
    for (int i = 0; i < WIDTH; i++) {
        
        for (int j = 0; j < HEIGHT; j++) {
            map[j][i] = 1;
        }
    }
}

void create_corridor(Room* room1, Room* room2, int corridor_width, int HEIGHT, int WIDTH) {
    

    static int prev_x = -1;
    static int prev_y = -1;
    
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
    
    // Horizontal
    if (dx != 0) {
        
        int step = (dx > 0) ? 1 : -1;
        
        for (int x = source_x; x != target_x + step; x += step) {
            
            for (int y = source_y - corridor_width / 2; y <= source_y + corridor_width / 2; y++) {
                
                if (y > 0 && y < HEIGHT-1) {
                    if (y == source_y) {
                        map[y][x] = 2; 
                    } 
                    else {
                        map[y][x] = 0; 
                    }
                }
            }
        }

        prev_x = target_x;
        prev_y = source_y;
    }
    
    // Vertical
    if (dy != 0) {
        int step = (dy > 0) ? 1 : -1;
    
        for (int y = source_y; y != target_y + step; y += step) {
            
            for (int x = source_x - corridor_width / 2; x <= source_x + corridor_width / 2; x++) {
                
                if (x > 0 && x < WIDTH-1 && map[y][x] != 2) {
                    
                    if (x > source_x - corridor_width / 2 && x < source_x + corridor_width / 2) {
                        map[y][x] = 2;
                    } 
                    else {
                        map[y][x] = 0;
                    }
                }
            }
        }
    
        prev_x = source_x;
        prev_y = target_y;
    }

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


void split_room_sewers(Room* room, int HEIGHT, int WIDTH) {
    
    int split_horizontal = rand() % 2; // Split either horizontally or vertically
    int max_size = (split_horizontal ? room->height : room->width); // Leave 1 cell border
    
    if (max_size < ROOM_MIN_WIDTH * 2 || max_size < ROOM_MIN_HEIGHT * 2) {
        // Room is too small to split, so return
        return;
    }
    
    // Randomly determines the position of the split
    int split_position = (rand() % (max_size - ROOM_MIN_WIDTH)) + ROOM_MIN_WIDTH + (split_horizontal ? room->y : room->x);

    // Randomize the order in which the rooms are split
    Room* left_room = NULL;
    Room* right_room = NULL;
    
    // Creates two new sub-rooms based on the split and randomly shuffles the order of the sub-rooms
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
    // If the sub-rooms are too small to split, the function returns without creating a corridor
    if (left_room != NULL && right_room != NULL &&
        ((split_horizontal && (left_room->height < ROOM_MIN_HEIGHT * 2 || right_room->height < ROOM_MIN_HEIGHT * 2))
        || (!split_horizontal && (left_room->width < ROOM_MIN_WIDTH * 2 || right_room->width < ROOM_MIN_WIDTH * 2)))) {
        free(left_room);
        free(right_room);
        return;
    }

    // Otherwise, creates a corridor between the sub-rooms, and recursively calls split_room_sewers() on each of the sub-rooms
    if (left_room != NULL && right_room != NULL) {
        create_corridor(left_room, right_room, CORRIDOR_WIDTH, HEIGHT, WIDTH);
        split_room_sewers(left_room, HEIGHT, WIDTH);
        split_room_sewers(right_room, HEIGHT, WIDTH);
    }
}

void create_sewers(int HEIGHT, int WIDTH, int beginY, int beginX){
    
    init_maze(HEIGHT, WIDTH);

    // The root_room is the starting point of the map generation process, which is a single room that occupies the entire map
    Room* root_room = create_room_sewers(0, 0, WIDTH, HEIGHT);

    split_room_sewers(root_room, HEIGHT, WIDTH);

    Room* rooms = (Room*)malloc(NUM_ROOMS * sizeof(Room));
    Room* prev_room = NULL;
    
    for (int i = 0; i < NUM_ROOMS; i++) {
        
        int room_width = rand() % (WIDTH - ROOM_MIN_WIDTH) + ROOM_MIN_WIDTH;
        int room_height = rand() % (HEIGHT - ROOM_MIN_HEIGHT) + ROOM_MIN_HEIGHT;
        int room_x = rand() % (WIDTH - room_width);
        int room_y = rand() % (HEIGHT - room_height);
        
        rooms[i] = *create_room_sewers(room_x, room_y, room_width, room_height);

        if (prev_room != NULL) {
            create_corridor(prev_room, &rooms[i], CORRIDOR_WIDTH, HEIGHT, WIDTH);
        }
        
        prev_room = &rooms[i];
    }

    free(rooms->left);
    free(rooms->right);
    free(rooms);
    
    for (int i = 0; i < HEIGHT; i++) {
        map[i][0] = 1;
        map[i][WIDTH-1] = 1;
    }
    
    for (int j = 0; j < WIDTH; j++) {
        map[0][j] = 1;
        map[HEIGHT-1][j] = 1;
    }

    make_borders(map, HEIGHT, WIDTH);
    print_sewers(HEIGHT, WIDTH, beginY, beginX);
}
#pragma endregion


//==========================
// Asylum
//==========================
#pragma region Asylum

#define MAX_ROOMS 10
#define MIN_SIZE 13
#define MAX_SIZE 15

// Initialize the "map" array with all cells set to 1 (representing walls).
void init_asylum(int HEIGHT, int WIDTH){
    
    // Allocate space for matrix "map"
    map = (int **)malloc(HEIGHT * sizeof(int *));
    for (int i = 0; i < HEIGHT; i++) {
        map[i] = (int *)malloc(WIDTH * sizeof(int));
    }
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            map[y][x] = 1;
        }
    }
}

// Create a Room struct with random values for its x and y position, width, and height.
Room create_room(int HEIGHT, int WIDTH) {
    
    Room room;

    room.width = rand() % (MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE; // random area
    room.height = rand() % (MAX_SIZE - MIN_SIZE - 1) + MIN_SIZE;
    room.x = rand() % (abs(WIDTH - room.width-1)) + 1; // random location
    room.y = rand() % (abs(HEIGHT - room.height-1)) + 1;


    return room;
}

// Carve out the specified room in the "map" array by setting all cells inside the room to 0.
void carve_room(Room room) {
    
    for (int y = room.y; y < room.y + room.height; y++) {
        
        for (int x = room.x; x < room.x + room.width; x++) {
            map[y][x] = 0;
        }
    }
}

// Connect two rooms by carving out a corridor between them.
void connect_rooms(Room source, Room destination) {
    
    int x = source.x + (source.width / 2);
    int y = source.y + (source.height / 2);

    while (x != destination.x + (destination.width / 2)) {
        
        for (int i = y - 2; i <= y + 2; i++) {
            map[i][x] = 0;
        }
        x += (x < destination.x + (destination.width / 2)) ? 1 : -1;
    }

    while (y != destination.y + (destination.height / 2)) {
        
        for (int i = x - 2; i <= x + 2; i++) {
            map[y][i] = 0;
        }
        y += (y < destination.y + (destination.height / 2)) ? 1 : -1;
    }
}


// Carve out corridors to connect all the rooms in the "rooms" array.
void carve_corridors(Room rooms[], int num_rooms) {
    
    for (int i = 0; i < num_rooms - 1; i++) {
        connect_rooms(rooms[i], rooms[i + 1]);
    }
}

void blood_stains_asylum(int HEIGHT, int WIDTH) {

    // Random stains in floor
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (map[y][x] == 0 && rand() % 100 < 10){ // % of blood
                map[y][x] = 4;
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
                        
                        if (map[y+dy][x+dx] == 0) {
                            
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
                
                map[y][x] = new_asylum[y][x];
            }
        }
    }

    // Redraw the stains with updated positions
    for (int y = 1; y < HEIGHT - 1; y++) {
        
        for (int x = 1; x < WIDTH - 1; x++) {
            
            if (map[y][x] == 0) {
                
                if (rand() % 100 < 10){ // % of blood
                    map[y][x] = 4;
                }
            }
        }
    }
}

void create_asylum(int HEIGHT, int WIDTH, int beginY, int beginX){
 
    init_asylum(HEIGHT, WIDTH);

    Room rooms[MAX_ROOMS];
    int num_rooms = rand() % (MAX_ROOMS - 1) + 2;

    for (int i = 0; i < num_rooms; i++) {
        
        rooms[i] = create_room(HEIGHT, WIDTH);
        carve_room(rooms[i]);
    }

    carve_corridors(rooms, num_rooms);
    blood_stains_asylum(HEIGHT, WIDTH);

    // Check top and bottom borders
    for (int x = 0; x < WIDTH; x++) {
        if (map[0][x] == 0) {
            map[0][x] = 1;
        }
        if (map[HEIGHT - 1][x] == 0) {
            map[HEIGHT - 1][x] = 1;
        }
    }
    
    // Check left and right borders (excluding corners)
    for (int y = 1; y < HEIGHT - 1; y++) {
        if (map[y][0] == 0) {
            map[y][0] = 1;
        }
        if (map[y][WIDTH - 1] == 0) {
            map[y][WIDTH - 1] = 1;
        }
    }

    make_borders(map, HEIGHT, WIDTH);
    print_asylum(HEIGHT, WIDTH, beginY, beginX);
}

//====================================
// Random Map
//====================================

int create_random_map(int HEIGHT, int WIDTH, int beginY, int beginX){

    int random = rand() % 3;

    if(random == 0){
        create_dungeon(HEIGHT,WIDTH, beginY, beginX);
        return 1;
    }
    if(random == 1){ 
        create_sewers(HEIGHT, WIDTH, beginY, beginX);
        return 2;
    }
    //if(random == 2)
    {
        create_asylum(HEIGHT, WIDTH, beginY, beginX);
        return 3;
    }
}

//====================================
// Valid Map
//====================================
	
int valid_map(int HEIGHT, int WIDTH){
    
    for (int y = 0; y < HEIGHT; y++){
		
		for (int x = 0; x < WIDTH; x++){
			
			if (map[y][x] == 0){
				return 1;
			}
		}	
	}
    
    return 0;
}