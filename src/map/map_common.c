#include "map_common.h"

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