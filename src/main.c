#include <stdio.h>
#include <libgen.h>
#include "common.h"
#include "map/map.h"
#include "entity/entity.h"
#include "entity/player/player.h"
#include "render/render.h"
#include "gameloop/gameloop.h"

// char BIN_PATH[PATH_MAX];
char* BIN_PATH;
int BIN_PATH_LEN;
char ASSET_DIR[PATH_MAX];
int ASSET_DIR_LEN = PATH_MAX;

int main() {
    // Setup Paths5
    char BIN_PATH_TMP[PATH_MAX];
    readlink("/proc/self/exe", BIN_PATH_TMP, PATH_MAX);

    BIN_PATH = dirname(BIN_PATH_TMP);
    BIN_PATH_LEN = strlen(BIN_PATH);

    strncpy(ASSET_DIR, BIN_PATH, BIN_PATH_LEN);
    strcat(ASSET_DIR, "/assets");
    
    printf("Hello, world!\nEXE PATH: %s\n", BIN_PATH);

    // Coords test = defaultCoords();
    // printf("Default coords: %d %d\n", test->x, test->y);

    // Entity test = defaultEntity();
    // printf("Default coords: %d %d\n", test->coords->x, test->coords->y);
    // printf("Entity: %d %d\n", test->health, test->armor);

    // Player test = defaultPlayer();
    // printf("Player coords: %d %d\n", test->entity->coords->x, test->entity->coords->y);
    // printf("Player 1: %d %d %d\n", test->entity->health, test->entity->armor, test->entity->maxHealth);

    // damageEntity(test->entity, 1);
    // printf("Player 2: %d %d\n", test->entity->health, test->entity->armor);
    // healEntity(test->entity, 2);
    // printf("Player 3: %d %d\n", test->entity->health, test->entity->armor);

    init_gameloop();
    render();
    
    return 0;
}