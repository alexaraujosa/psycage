#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <time.h>
#include "common.h"
#include "util/string.h"
#include "gameloop/gameloop.h"
#include "render/render.h"

int EXIT = FALSE;

// char BIN_PATH[PATH_MAX];
char* BIN_PATH;
int BIN_PATH_LEN;
char ASSET_DIR[PATH_MAX];
int ASSET_DIR_LEN = PATH_MAX;
FILE* dbgOut;
int DEBUG_LEVEL = 0;

int main(int argc, char *argv[]) {
    // Parse arguments
    for (int i = 1; i < argc; i++) {

        // Debug level flag
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
           char *endptr;
            int level = strtol(argv[i + 1], &endptr, 10);

            if (*endptr == '\0') {
                
                DEBUG_LEVEL = level;
                break;
            }
        }
    }

    // Setup Paths
    char BIN_PATH_TMP[PATH_MAX];
    if (!readlink("/proc/self/exe", BIN_PATH_TMP, PATH_MAX)) {
        printf("Unable to read current executable path.\n");
        exit(1);
    }

    BIN_PATH = dirname(BIN_PATH_TMP);
    BIN_PATH_LEN = strlen(BIN_PATH);

    strncpy(ASSET_DIR, BIN_PATH, BIN_PATH_LEN);
    strcat(ASSET_DIR, "/assets");

    // Setup Debug Log File
	char dbgout_path[PATH_MAX];
    snprintf(dbgout_path, sizeof(dbgout_path), "%.*s/logs/dbg-", BIN_PATH_LEN, BIN_PATH);

	time_t now = time(NULL);
    struct tm* tmInfo = localtime(&now);
    size_t dateFormatSize = 20;
    size_t new_dbgout_len = BIN_PATH_LEN + sizeof("/logs/dbg-") - 1 + dateFormatSize;

    char* new_dbgout_path = malloc(new_dbgout_len);
    if (new_dbgout_path == NULL) {
        fprintf(stderr, "Unable to initialize game: Failed to generate log path.\n");
        return 1;
    }
    strcpy(new_dbgout_path, dbgout_path);
    strftime(new_dbgout_path + new_dbgout_len - dateFormatSize, dateFormatSize, "%Y-%m-%dT%H:%M:%S", tmInfo);
    strcat(new_dbgout_path, ".log");

	if (createParentFolder(new_dbgout_path) != 0) {
        printf("Unable to initialize game: Unable to create log folder.\n");
        exit(1);
    }

    dbgOut = fopen(new_dbgout_path, "a");

    debug_file(dbgOut, 0, "Logger initialized.\n");
    
    // Initialize random
    srandom(time(NULL));

    // Initialize renderer
    debug_file(dbgOut, 0, "Initializing renderer...\n");
    Renderstate rs = init_render();
    debug_file(dbgOut, 0, "Renderer initialized successfully.\n");

    // Initialize items
    debug_file(dbgOut, 0, "Initializing items...\n");
    load_items();
    
    // Initialize gameloop
    debug_file(dbgOut, 0, "Initializing gameloop...\n");
    Gamestate gs = init_gameloop();

    debug_file(dbgOut, 0, "Gameloop initialized succesfully.\n");
    
    player_spawn(gs->player, map, g_renderstate->nrows-1, g_renderstate->ncols-2); // spawn

    // Event Loop
    debug_file(dbgOut, 0, "Event loop started.\n");
    while (!EXIT) {
        tick();
        render(gs);
    }

    endwin();
    
    return 0;
}
