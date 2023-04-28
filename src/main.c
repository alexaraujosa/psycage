#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <time.h>
#include "common.h"
#include "util/string.h"
#include "gameloop/gameloop.h"
#include "render/render.h"

// char BIN_PATH[PATH_MAX];
char* BIN_PATH;
int BIN_PATH_LEN;
char ASSET_DIR[PATH_MAX];
int ASSET_DIR_LEN = PATH_MAX;

int main() {
    // Setup Paths
    char BIN_PATH_TMP[PATH_MAX];
    if (!readlink("/proc/self/exe", BIN_PATH_TMP, PATH_MAX)) {
        printf("Unable to read current executable path.\n");
        exit(1);
    }

    BIN_PATH = dirname(BIN_PATH_TMP);
    BIN_PATH_LEN = strlen_cp(BIN_PATH);

    strncpy(ASSET_DIR, BIN_PATH, BIN_PATH_LEN);
    strcat(ASSET_DIR, "/assets");
    
    srandom(time(NULL));

    Renderstate rs = init_render();
    Gamestate gs = init_gameloop();
    
    gs->player->entity->coords->x = rs->nrows / 2;
    gs->player->entity->coords->y = rs->ncols / 2;

    while (1) {
        tick();
        render(gs);
    }
    
    return 0;
}
