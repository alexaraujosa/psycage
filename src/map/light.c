#include "light.h"

//====================================
// Light
//====================================

#define PI 3.14159265
#define RADIUS 20

int **visible = NULL;

void init_light_map(int HEIGHT, int WIDTH) {
    
    visible = (int **)malloc(HEIGHT * sizeof(int *));
    
    for (int i = 0; i < HEIGHT; i++) {
        
        visible[i] = (int *)malloc(WIDTH * sizeof(int));
        
        for (int j = 0; j < WIDTH; j++) {
            visible[i][j] = 0;
        }
    }
}

void cast_light(int x, int y, int row, double start_slope, double end_slope, int **map, int octant, int HEIGHT, int WIDTH) {
    

    // Check if the start slope is less than the end slope, if it is then return because no further cells need to be checked in this direction
    if (start_slope < end_slope) {
        return;
    }

    // Initialize the next start slope to be equal to the current start slope
    double next_start_slope = start_slope;
    

    for (int i = row; i <= RADIUS; i++) {
        
        int blocked = 0;
        
        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            
            // For each cell in the current row being checked, calculate the slope of its left and right boundaries
            double l_slope = (dx - 0.5) / (dy + 0.5);
            double r_slope = (dx + 0.5) / (dy - 0.5);
            
            // If the start slope is less than the right slope, skip this cell and move to the next one
            if (start_slope < r_slope) {
                continue;
            }

            // If the end slope is greater than the left slope, break out of the cell loop because all remaining cells in this row are blocked
            if (end_slope > l_slope) {
                break;
            }
            
            int current_x, current_y;
            

            // Calculate the current x and y coordinates based on the octant being checked and the current dx and dy values
            switch (octant) {
                case 0:
                    current_x = x + 2*dx;
                    current_y = y - dy;
                    break;
                case 1:
                    current_x = x + 2*dy;
                    current_y = y - dx;
                    break;
                case 2:
                    current_x = x + 2*dy;
                    current_y = y + dx;
                    break;
                case 3:
                    current_x = x + 2*dx;
                    current_y = y + dy;
                    break;
                case 4:
                    current_x = x - 2*dx;
                    current_y = y + dy;
                    break;
                case 5:
                    current_x = x - 2*dy;
                    current_y = y + dx;
                    break;
                case 6:
                    current_x = x - 2*dy;
                    current_y = y - dx;
                    break;
                case 7:
                    current_x = x - 2*dx;
                    current_y = y - dy;
                    break;
            }
            
            // If the current coordinates are outside the bounds of the map, skip this cell and move to the next one
            if (current_x < 0 || current_y < 0 || current_x >= WIDTH|| current_y >= HEIGHT) {
                continue;
            }
            
            // Calculate the distance from the starting point to the current cell
            double distance = sqrt(dx * dx + dy * dy);

            // If the distance is less than or equal to the radius, set the visibility of the current cell to 1
            if (distance <= RADIUS) {
                
                visible[current_y][current_x-1] = 1;
                visible[current_y][current_x] = 1;
                visible[current_y][current_x+1] = 1;
            }
            
            // If blocked is true, check if the current cell is a wall, if it is update the next_start_slope variable and continue to the next cell
            // If it is not, update blocked to false and reset the start slope to the next_start_slope value
            if (blocked) {
                
                if (current_x < WIDTH && (map[current_y][current_x] == 1 || map[current_y][current_x] == 3)) {
                    
                    next_start_slope = r_slope;
                    continue;
                }
                else {
                    blocked = 0;
                    start_slope = next_start_slope;
                }
            }
            // If blocked is false, check if the current cell is a wall or obstacle and if the current row is less than the radius
            // If it is, set blocked to true, recursively call the cast_light function with the updated row and slope values, and update next_start_slope
            else {
                if (current_x < WIDTH && (map[current_y][current_x] == 1 || map[current_y][current_x] == 3) && i < RADIUS) {
                    blocked = 1;
                    cast_light(x, y, i + 1, start_slope, l_slope, map, octant, HEIGHT, WIDTH);
                    next_start_slope = r_slope;
                }
            }
        }
        
        // If blocked is true, break out of the row loop because all remaining cells in this row are blocked
        if (blocked) {
            break;
        }
    }
}

void calculate_visibility(int x, int y, int **map, int HEIGHT, int WIDTH){
    
    if (visible != NULL) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                visible[i][j] = 0;
            }
        }

        //free(visible);
    }

    // init_light_map(HEIGHT, WIDTH);
    
    for (int octant = 0; octant < 8; octant++) {
        cast_light(x, y, 1, 1.0, 0.0, map, octant, HEIGHT, WIDTH);
    }
    
    visible[y][x] = 1;
}

void print_light(WINDOW* win, int HEIGHT, int WIDTH){
    
    for (int y = 0; y < HEIGHT; y++) {
        
        for (int x = 0; x < WIDTH; x++) {
            
            if(visible[y][x] == 0){
                
                mvwaddch(win, y, x, ' ' | COLOR_BLACK);
            }
            else{

                mvwprintw(win, y, x, '\0');
            }
        }
        
        wprintw(win, "\n");
    }

    wrefresh(win);
}








