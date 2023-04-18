#include <stdlib.h>
#include "map.h"

Coords defaultCoords() {
    Coords coords = (Coords)malloc(sizeof(Coords));
    if (coords == NULL) return NULL;

    coords->x = 0;
    coords->y = 0;

    return coords;
}

void destroyCoords(Coords coords) {
    free(coords);
}