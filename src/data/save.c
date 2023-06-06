#include "save.h"
#include <unistd.h>
#include <stdio.h>
#include "../render/render.h"
#include "../gameloop/gameloop.h"

extern FILE* dbgOut;

static unsigned short SAVE_VERSION = 10;

int get_savepath(char *save_path, int num_save) {
    int res = snprintf(save_path, PATH_MAX, "%s/saves/save%d.dat", BIN_PATH, num_save);

    if (res >= 0 && res < PATH_MAX) return 0;
    else return 1;
}

int verify_Save(int num_save) {
    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) return 1;

    if(access(save_path, F_OK) != -1) return 0;
    
    return 1;
}


int delete_Save(int num_save) {

    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) {
        debug_file(dbgOut, 0, "Error while deleting save file (index %d): Cannot fetch saves directory path.\n", num_save);
        return 1;
    }

    if(verify_Save(num_save) == 0) {

        // if(access(save_path, F_OK) != -1) {

            int verifier = remove(save_path);

            if(verifier == -1) {
                debug_file(dbgOut, 0, "Cannot delete save file %s: unknown error.\n", save_path);
                return 1;
            }

            debug_file(dbgOut, 0, "Successfully deleted save file %s.\n", save_path);
            return 0;

        // }

    } else {
        debug_file(dbgOut, 0, "Cannot delete save file %s: missing save file.\n", save_path);
        return 1;
    }

    return 1;
}

#pragma region Create Save

#pragma region Writers
#pragma region Primitives
int _save_write_int(FILE* save, int data) {
    if (save == NULL) return 1;

    debug_file(dbgOut, 1, "Writing integer property to save file...\n");

    if (fwrite(&data, sizeof(int), 1, save) != 1) {
        debug_file(dbgOut, 1, "- Failed to write integer property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_unsigned_int(FILE* save, unsigned int data) {
    if (save == NULL) return 1;

    debug_file(dbgOut, 1, "Writing unsigned integer to save file...\n");

    if (fwrite(&data, sizeof(unsigned int), 1, save) != 1) {
        debug_file(dbgOut, 1, "- Failed to write unsigned integer property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_unsigned_long_long(FILE* save, unsigned long long data) {
    if (save == NULL) return 1;

    debug_file(dbgOut, 1, "Writing unsigned long long to save file...\n");

    if (fwrite(&data, sizeof(unsigned long long), 1, save) != 1) {
        debug_file(dbgOut, 1, "- Failed to write unsigned long long property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_matrix(FILE* save, int** matrix, int rows, int cols) {
    debug_file(dbgOut, 1, "Writing matrix to save file...\n");

    if (_save_write_int(save, rows) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'rows' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, cols) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'cols' property to save file.\n");
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        if (fwrite(matrix[i], sizeof(int), cols, save) != (size_t)cols) {
            debug_file(dbgOut, 1, "- Failed to write matrix data to save file.\n");
            return 1;
        }
    }

    return 0;
}
#pragma endregion

#pragma region Game Structs
int _save_write_coords(FILE* save, Coords coords) {
    if (save == NULL || coords == NULL) return 1;

    debug_file(dbgOut, 1, "Writing coords to save file...\n");

    if (_save_write_int(save, coords->x) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'x' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, coords->y) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'y' property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_entity(FILE* save, Entity entity) {
    if (save == NULL || entity == NULL) return 1;

    debug_file(dbgOut, 1, "Writing entity to save file...\n");

    if (_save_write_coords(save, entity->coords) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'coords' property to save file.\n");
        return 1;
    }

    if (_save_write_unsigned_int(save, entity->maxHealth) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'maxHealth' property to save file.\n");
        return 1;
    }

    if (_save_write_unsigned_long_long(save, entity->health) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'health' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, entity->armor) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'armor' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, entity->basedamage) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'basedamage' property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_player(FILE* save, Player player) {
    if (save == NULL || player == NULL) return 1;

    debug_file(dbgOut, 1, "Writing player to save file...\n");

    if (_save_write_entity(save, player->entity) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'entity' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->level) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'level' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->kills) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'kills' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->xp) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'xp' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->last_direction) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'last_direction' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->class) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'class' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->radius) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'radius' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->sanity) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'sanity' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->candle_fuel) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'candle_fuel' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->current_candle) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'current_candle' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, player->hasUltimate) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'radius' hasUltimate to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_mob(FILE* save, Mob mob) {
    if (save == NULL || mob == NULL) return 1;

    debug_file(dbgOut, 1, "Writing mob to save file...\n");

    if (_save_write_entity(save, mob->entity) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'entity' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, mob->moveCooldown) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'moveCooldown' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, mob->lastMove) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'lastMove' property to save file.\n");
        return 1;
    }

    if (_save_write_int(save, mob->hasAI) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'hasAI' property to save file.\n");
        return 1;
    }

    return 0;
}

int _save_write_mobs(FILE* save, Mob* mobs, int count) {
    if (save == NULL || mobs == NULL || count <= 0) return 1;

    debug_file(dbgOut, 1, "Writing mobs to save file...\n");

    if (_save_write_int(save, count) != 0) {
        debug_file(dbgOut, 1, "- Failed to write 'count' property to save file.\n");
        return 1;
    }

    for (int i = 0; i < count; i++) {
        if (_save_write_mob(save, mobs[i]) != 0) {
            debug_file(dbgOut, 1, "- Failed to write mob %d to save file.\n", i);
            return 1;
        }
    }

    return 0;
}
#pragma endregion
#pragma endregion

int create_Save(int num_save) {
    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) {
        debug_file(dbgOut, 0, "Error while creating save file (index %d): Cannot fetch saves directory path.\n", num_save);
        return 1;
    }

    if (createParentFolder(save_path)) {
        debug_file(dbgOut, 0, "Error while creating save file %s: Cannot create saves directory.\n", save_path);
        return 1;
    }

    FILE *save = fopen(save_path, "wb");

    if(save == NULL) {
        debug_file(dbgOut, 0, "Error while creating save file %s: Cannot create save file.\n", save_path);
        return 1;
    }

    debug_file(dbgOut, 0, "Starting save data write.\n");

    // Save version
    debug_file(dbgOut, 1, "- Saving version.\n");
    fwrite(&SAVE_VERSION, sizeof(unsigned short), 1, save);
    debug_file(dbgOut, 1, "- Saved version.\n");

    // Player
    debug_file(dbgOut, 1, "- Saving player.\n");
    _save_write_player(save, g_gamestate->player);
    debug_file(dbgOut, 1, "- Saved player.\n");

    // Mobs
    debug_file(dbgOut, 1, "- Saving mobs.\n");
    _save_write_mobs(save, g_gamestate->mobs, g_gamestate->mob_count);
    debug_file(dbgOut, 1, "- Saved mobs.\n");

    debug_file(dbgOut, 1, "- Saving map.\n");
    _save_write_int(save, find_map);

    debug_file(dbgOut, 1, "-- Saving map data.\n");
    _save_write_matrix(save, map, ALTURA_JOGO, LARGURA_JOGO);
    debug_file(dbgOut, 1, "-- Saved map data.\n");

    debug_file(dbgOut, 1, "-- Saving lightmap data.\n");
    _save_write_matrix(save, visible, ALTURA_JOGO, LARGURA_JOGO);
    debug_file(dbgOut, 1, "-- Saved lightmap data.\n");

    debug_file(dbgOut, 1, "-- Saving map_footprint data.\n");
    _save_write_matrix(save, map_footprint, ALTURA_JOGO, LARGURA_JOGO);
    debug_file(dbgOut, 1, "-- Saved map_footprint data.\n");

    debug_file(dbgOut, 1, "- Saved map.\n");

    fclose(save);

    debug_file(dbgOut, 0, "Successfully saved file %s\n", save_path);

    return 0;
}
#pragma endregion

#pragma region Load Save

#pragma region Readers
#pragma region Primitives
int _save_read_int(FILE* save, int* data) {
    if (save == NULL || data == NULL) return 1;

    if (fread(data, sizeof(int), 1, save) != 1) {
        debug_file(dbgOut, 1, "Failed to read integer property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_unsigned_int(FILE* save, unsigned int* data) {
    if (save == NULL || data == NULL) return 1;

    if (fread(data, sizeof(unsigned int), 1, save) != 1) {
        debug_file(dbgOut, 1, "Failed to read unsigned integer property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_unsigned_long_long(FILE* save, unsigned long long* data) {
    if (save == NULL || data == NULL) return 1;

    if (fread(data, sizeof(unsigned long long), 1, save) != 1) {
        debug_file(dbgOut, 1, "Failed to read unsigned long long property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_matrix(FILE* save, int* rows, int* cols, int*** matrix) {
    if (fread(rows, sizeof(int), 1, save) != 1 || fread(cols, sizeof(int), 1, save) != 1) {
        debug_file(dbgOut, 1, "Failed to read matrix dimensions from save file.\n");
        return 1;
    }

    *matrix = malloc((*rows) * sizeof(int*));
    if (*matrix == NULL) {
        debug_file(dbgOut, 1, "Failed to allocate memory for matrix.\n");
        return 1;
    }

    for (int i = 0; i < *rows; i++) {
        (*matrix)[i] = malloc((*cols) * sizeof(int));
        if ((*matrix)[i] == NULL) {
            debug_file(dbgOut, 1, "Failed to allocate memory for matrix row.\n");

            for (int j = 0; j < i; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return 1;
        }
    }

    debug_file(dbgOut, 1, "Attempting to load matrix with dimensions %dx%d.\n", *rows, *cols);

    for (int i = 0; i < *rows; i++) {
        if (fread((*matrix)[i], sizeof(int), *cols, save) != (size_t)(*cols)) {
            debug_file(dbgOut, 1, "Failed to read matrix row from save file.\n");

            for (int j = 0; j < *rows; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return 1;
        }
    }

    return 0;
}
#pragma endregion

#pragma region Game Structs
int _save_read_coords(FILE* save, Coords coords) {
    if (save == NULL || coords == NULL) return 1;

    debug_file(dbgOut, 1, "Reading coords from save file...\n");

    if (_save_read_int(save, &(coords->x)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'x' coordinate from save file.\n");
        return 1;
    }

    if (_save_read_int(save, &(coords->y)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'y' coordinate from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_entity(FILE* save, Entity entity) {
    if (save == NULL || entity == NULL) return 1;

    debug_file(dbgOut, 1, "Reading entity from save file...\n");

    if (_save_read_coords(save, entity->coords) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'coords' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'maxHealth' from save file...\n");
    if (_save_read_unsigned_int(save, &(entity->maxHealth)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'maxHealth' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'health' from save file...\n");
    if (_save_read_unsigned_long_long(save, &(entity->health)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'health' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'armor' from save file...\n");
    if (_save_read_int(save, &(entity->armor)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'armor' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'basedamage' from save file...\n");
    if (_save_read_int(save, &(entity->basedamage)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'basedamage' property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_player(FILE* save, Player player) {
    if (save == NULL || player == NULL) return 1;

    debug_file(dbgOut, 1, "Reading player from save file...\n");

    if (_save_read_entity(save, player->entity) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'entity' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'level' from save file...\n");
    if (_save_read_int(save, &(player->level)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'level' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'kills' from save file...\n");
    if (_save_read_int(save, &(player->kills)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'kills' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'xp' from save file...\n");
    if (_save_read_int(save, &(player->xp)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'xp' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'last_direction' from save file...\n");
    if (_save_read_int(save, &(player->last_direction)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'last_direction' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'class' from save file...\n");
    if (_save_read_int(save, &(player->class)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'class' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'radius' from save file...\n");
    if (_save_read_int(save, &(player->radius)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'radius' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'sanity' from save file...\n");
    if (_save_read_int(save, &(player->sanity)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'radius' sanity from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'candle_fuel' from save file...\n");
    if (_save_read_int(save, &(player->candle_fuel)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'candle_fuel' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'current_candle' from save file...\n");
    if (_save_read_int(save, &(player->current_candle)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'current_candle' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'hasUltimate' from save file...\n");
    if (_save_read_int(save, &(player->hasUltimate)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'hasUltimate' property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_mob(FILE* save, Mob mob) {
    if (save == NULL || mob == NULL) return 1;

    debug_file(dbgOut, 1, "Reading mob from save file...\n");

    if (_save_read_entity(save, mob->entity) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'entity' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'moveCooldown' from save file...\n");
    if (_save_read_int(save, &(mob->moveCooldown)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'moveCooldown' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'lastMove' from save file...\n");
    if (_save_read_int(save, &(mob->lastMove)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'lastMove' property from save file.\n");
        return 1;
    }

    debug_file(dbgOut, 1, "- Reading 'hasAI' from save file...\n");
    if (_save_read_int(save, &(mob->hasAI)) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'hasAI' property from save file.\n");
        return 1;
    }

    return 0;
}

int _save_read_mobs(FILE* save, Mob** dest, int* mobCount) {
    if (save == NULL || dest == NULL || mobCount == NULL) return 1;

    debug_file(dbgOut, 1, "Reading mobs from save file...\n");

    int count = 0;
    if (_save_read_int(save, &count) != 0) {
        debug_file(dbgOut, 1, "- Failed to read 'count' property from save file.\n");
        return 1;
    }

    Mob* mobs = (Mob*)malloc(count * sizeof(Mob));
    if (mobs == NULL) {
        debug_file(dbgOut, 1, "- Unable to allocate memory for %d elements.\n", count);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        mobs[i] = defaultMob();
        if (_save_read_mob(save, mobs[i]) != 0) {
            debug_file(dbgOut, 1, "- Failed to read mob at index %d from save file.\n", i);
            free(mobs);
            return 1;
        }
    }

    *mobCount = count;
    *dest = mobs;

    return 0;
}
#pragma endregion
#pragma endregion

int load_save(int num_save) {
    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) {
        debug_file(dbgOut, 0, "Error while loading save file (index %d): Cannot fetch saves directory path.\n", num_save);
        return 1;
    }

    FILE *save = fopen(save_path, "rb");

    struct save_data* sd = (struct save_data*)malloc(sizeof(struct save_data));

    debug_file(dbgOut, 0, "Starting save data load.\n");

    // Save version
    debug_file(dbgOut, 1, "- Loading version.\n");
    fread(&sd->version, sizeof(unsigned short), 1, save);
    debug_file(dbgOut, 1, "- Loaded version.\n");

    debug_file(dbgOut, 1, "- Verifying save version.\n");
    if (sd->version != SAVE_VERSION) {
        debug_file(dbgOut, 0, "Error while loading save file %s: Invalid version.\n", save_path);
        return 1;
    }
    debug_file(dbgOut, 1, "- Save version is valid.\n");

    // Player
    debug_file(dbgOut, 1, "- Loading player.\n");

    Player pl = defaultPlayer();
    _save_read_player(save, pl);
    destroyPlayer(g_gamestate->player);
    g_gamestate->player = pl;

    debug_file(dbgOut, 1, "- Loaded player.\n");

    // Mobs
    debug_file(dbgOut, 1, "- Loading mobs.\n");

    Mob* mobs = NULL;
    int count = -1;
    _save_read_mobs(save, &mobs, &count);

    for (int i = 0; i < g_gamestate->mob_count; i++) {
        destroyMob(g_gamestate->mobs[i]);
    }
    
    g_gamestate->mob_count = count;
    g_gamestate->mobs = mobs;

    debug_file(dbgOut, 1, "- Loaded mobs.\n");

    // debug_file(dbgOut, 2, "- Cleaning old map data...\n");
    
    // debug_file(dbgOut, 2, "-- Cleaning old map footprint...\n");
    // for (int i = 0; i < ALTURA_JOGO; i++) {
    //     free(map_footprint[i]);
    // }
    // free(map_footprint);
    // debug_file(dbgOut, 2, "- Cleaned old map data.\n");

    debug_file(dbgOut, 1, "- Loading map.\n");
    int nfind_map = -1;
    fread(&nfind_map, sizeof(int), 1, save);

    int nrows, ncols;
    int** nmap = NULL;

    if (_save_read_matrix(save, &nrows, &ncols, &nmap)) {
        debug_file(dbgOut, 0, "Error while loading save file %s: Cannot load map.\n", save_path);
        return 1;
    }

    find_map = nfind_map;
    ALTURA_JOGO = nrows;
    LARGURA_JOGO = ncols;
    map = nmap;

    debug_file(dbgOut, 1, "- Loaded map.\n");

    debug_file(dbgOut, 0, "-- Clearing light map...\n");
    if (visible != NULL) {
        for (int i = 0; i < ALTURA_JOGO; i++) {
            for (int j = 0; j < LARGURA_JOGO; j++) {
                visible[i][j] = 0;
            }
        }
    }
    debug_file(dbgOut, 0, "-- Cleared light map.\n");

    debug_file(dbgOut, 0, "-- Loading light map...\n");
    int** lmap = NULL;
    int lm_rows = 0, lm_cols = 0;

    if (_save_read_matrix(save, &lm_rows, &lm_cols, &lmap)) {
        debug_file(dbgOut, 0, "Error while loading save file %s: Cannot load light map.\n", save_path);
        return 1;
    }

    visible = lmap;

    debug_file(dbgOut, 0, "-- Loaded light map.\n");

    // debug_file(dbgOut, 2, "-- Generating new map footprint...\n");
	// map_footprint = (int **)malloc((ALTURA_JOGO) * sizeof(int *));
    // for (int i = 0; i < ALTURA_JOGO; i++) {
    //     map_footprint[i] = (int *)malloc((LARGURA_JOGO) * sizeof(int));
    // }

	// for (int i = 0; i < ALTURA_JOGO; i++) {
	// 	for (int j = 0; j < (LARGURA_JOGO); j++) {
	// 		if (map[i][j] != 5) map_footprint[i][j] = map[i][j];
    //         else map_footprint[i][j] = 0;
	// 	}
	// }

    debug_file(dbgOut, 2, "-- Cleaning old map footprint...\n");
    for (int i = 0; i < ALTURA_JOGO; i++) {
        free(map_footprint[i]);
    }
    free(map_footprint);
    debug_file(dbgOut, 2, "-- Cleaned old map data.\n");

    debug_file(dbgOut, 0, "-- Loading map footprint...\n");
    int** mapf = NULL;
    int mf_rows = 0, mf_cols = 0;

    if (_save_read_matrix(save, &mf_rows, &mf_cols, &mapf)) {
        debug_file(dbgOut, 0, "Error while loading save file %s: Cannot load map footprint.\n", save_path);
        return 1;
    }

    map_footprint = mapf;

    debug_file(dbgOut, 0, "-- Loaded map footprint.\n");

    debug_file(dbgOut, 0, "Read save file %s, version=%d\n", save_path, sd->version);

    return 0;
}
#pragma endregion


