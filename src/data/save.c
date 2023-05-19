#include "save.h"
#include <unistd.h>
#include <stdio.h>
#include "../render/render.h"
#include "../gameloop/gameloop.h"

extern FILE* dbgOut;

static unsigned short SAVE_VERSION = 1;

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
// Primitive types
void _save_write_int(FILE* save, int data) {
    fwrite(&data, sizeof(int), 1, save);
}

void _save_write_charp(FILE* save, char* data) {
    size_t longStrLen = strlen(data);
    fwrite(&longStrLen, sizeof(size_t), 1, save);
    fwrite(data, sizeof(char), longStrLen, save);
}

void _save_write_matrix(FILE* save, int** matrix, int rows, int cols) {
    fwrite(&rows, sizeof(int), 1, save);
    fwrite(&cols, sizeof(int), 1, save);

    for (int i = 0; i < rows; i++) {
        fwrite(matrix[i], sizeof(int), cols, save);
    }
}


// Game Structs
void _save_write_coords(FILE* save, Coords coords) {
    _save_write_int(save, coords->x);
    _save_write_int(save, coords->y);
}

void _save_write_entity(FILE* save, Entity entity) {
    _save_write_coords(save, entity->coords);
    _save_write_int(save, entity->maxHealth);
    _save_write_int(save, entity->health);
    _save_write_int(save, entity->armor);
    _save_write_int(save, entity->basedamage);
    _save_write_int(save, entity->damage);
    _save_write_int(save, entity->dead);
}

void _save_write_player(FILE* save, Player player) {
    _save_write_entity(save, player->entity);
    _save_write_int(save, player->level);
    _save_write_int(save, player->kills);
    _save_write_int(save, player->xp);
    _save_write_int(save, player->last_direction);
    _save_write_int(save, player->cheats);
    _save_write_int(save, player->class);
    _save_write_int(save, player->radius);
}

void _save_write_mob(FILE* save, Mob mob) {
    _save_write_entity(save, mob->entity);
    _save_write_int(save, mob->moveCooldown);
    _save_write_int(save, mob->lastMove);
    _save_write_int(save, mob->hasAI);
}

void _save_write_mobs(FILE* save, Mob* mobs, int count) {
    _save_write_int(save, count);

    for (int i = 0; i < count; i++) {
        _save_write_mob(save, mobs[i]);
    }
}

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

    // fseek(save, 0, SEEK_SET);

    // fprintf(save, "#version=1");

    // fseek(save, 1, SEEK_CUR);

    // fprintf(save, "OLA");

    debug_file(dbgOut, 0, "Starting save data.\n");


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
    // _save_write_int(save, ALTURA_JOGO);
    // _save_write_int(save, LARGURA_JOGO);

    // _save_write_matrix(save, map, ALTURA_JOGO, LARGURA_JOGO);
    _save_write_matrix(save, map, g_renderstate->nrows - 1, LARGURA_JOGO);
    debug_file(dbgOut, 1, "- Saved map.\n");

    fclose(save);

    debug_file(dbgOut, 0, "Successfully saved file %s\n", save_path);

    return 0;
}
#pragma endregion

#pragma region Load Save
// Primitive types
void _save_read_int(FILE* save, int* data) {
    if (save == NULL || data == NULL) return;

    fread(data, sizeof(int), 1, save);
}

int _save_read_matrix(FILE* save, int* rows, int* cols, int*** matrix) {
    if (fread(rows, sizeof(int), 1, save) != 1 || fread(cols, sizeof(int), 1, save) != 1) {
        debug_file(dbgOut, 1, "Failed to read matrix dimensions from the file.\n");
        return -1;
    }

    *matrix = malloc((*rows) * sizeof(int*));
    if (*matrix == NULL) {
        debug_file(dbgOut, 1, "Failed to allocate memory for matrix.\n");
        return -1;
    }

    for (int i = 0; i < *rows; i++) {
        (*matrix)[i] = malloc((*cols) * sizeof(int));
        if ((*matrix)[i] == NULL) {
            debug_file(dbgOut, 1, "Failed to allocate memory for matrix row.\n");

            for (int j = 0; j < i; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return -1;
        }
    }

    for (int i = 0; i < *rows; i++) {
        if (fread((*matrix)[i], sizeof(int), *cols, save) != *cols) {
            debug_file(dbgOut, 1, "Failed to read matrix data from the file.\n");

            for (int j = 0; j < *rows; j++) {
                free((*matrix)[j]);
            }
            free(*matrix);
            return -1;
        }
    }

    return 0;
}

// Game Structs

void _save_read_coords(FILE* save, Coords coords) {
    fread(&coords->x, sizeof(int), 1, save);
    fread(&coords->y, sizeof(int), 1, save);
}

void _save_read_entity(FILE* save, Entity entity) {
    _save_read_coords(save, entity->coords);
    fread(&entity->maxHealth, sizeof(int), 1, save);
    fread(&entity->health, sizeof(int), 1, save);
    fread(&entity->armor, sizeof(int), 1, save);
    fread(&entity->basedamage, sizeof(int), 1, save);
    fread(&entity->damage, sizeof(int), 1, save);
    fread(&entity->dead, sizeof(int), 1, save);
}

void _save_read_player(FILE* save, Player player) {
    _save_read_entity(save, player->entity);
    fread(&player->level, sizeof(int), 1, save);
    fread(&player->kills, sizeof(int), 1, save);
    fread(&player->xp, sizeof(int), 1, save);
    fread(&player->last_direction, sizeof(int), 1, save);
    fread(&player->cheats, sizeof(int), 1, save);
    fread(&player->class, sizeof(int), 1, save);
    fread(&player->radius, sizeof(int), 1, save);
}

void _save_read_mob(FILE* save, Mob mob) {
    _save_read_entity(save, mob->entity);
    fread(&mob->moveCooldown, sizeof(int), 1, save);
    fread(&mob->lastMove, sizeof(int), 1, save);
    fread(&mob->hasAI, sizeof(int), 1, save);
}

int _save_read_mobs(FILE* save, Mob** dest, int* mobCount) {
    int count = 0;
    fread(&count, sizeof(int), 1, save);

    Mob* mobs = (Mob*)malloc(count * sizeof(Mob));
    if (mobs == NULL) {
        debug_file(dbgOut, 1, "Unable to read mob array from file: Unable to allocate memory for %d elements.\n", count);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        mobs[i] = defaultMob();
        _save_read_mob(save, mobs[i]);
    }

    *mobCount = count;
    *dest = mobs;
}

int load_save(int num_save) {
    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) {
        debug_file(dbgOut, 0, "Error while loading save file (index %d): Cannot fetch saves directory path.\n", num_save);
        return 1;
    }

    FILE *save = fopen(save_path, "rb");

    struct save_data* sd = (struct save_data*)malloc(sizeof(struct save_data));

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

    debug_file(dbgOut, 1, "- Loaded version.\n");

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
    ALTURA_JOGO = nrows - 1 - ALTURA_LOGO;
    LARGURA_JOGO = ncols;
    map = nmap;

    debug_file(dbgOut, 1, "- Loaded map.\n");

    debug_file(dbgOut, 0, " - Regenerating light map...\n");
    debug_file(dbgOut, 2, "-- Clearning old light map...\n");
    for (int i = 0; i < g_renderstate->nrows; i++) {
        free(visible[i]);
    }
    free(visible);
    visible = NULL;

    debug_file(dbgOut, 0, "-- Generating new light map...\n");
	init_light_map(g_renderstate->nrows, g_renderstate->ncols-2);

    debug_file(dbgOut, 1, "- Regenerating map footprint...\n");
    debug_file(dbgOut, 2, "-- Clearning old map footprint...\n");
    for (int i = 0; i < ALTURA_JOGO; i++) {
        free(map_footprint[i]);
    }
    free(map_footprint);

    debug_file(dbgOut, 2, "-- Generating new map footprint...\n");
	map_footprint = (int **)malloc((ALTURA_JOGO) * sizeof(int *));
    for (int i = 0; i < ALTURA_JOGO; i++) {
        map_footprint[i] = (int *)malloc((LARGURA_JOGO) * sizeof(int));
    }

	for (int i = 0; i < ALTURA_JOGO; i++) {
		for (int j = 0; j < (LARGURA_JOGO); j++) {
			if (map[i][j] != 5) map_footprint[i][j] = map[i][j];
            else map_footprint[i][j] = 0;
		}
	}

    debug_file(
        dbgOut, 
        0, 
        "Read save file %s with data\n - version=%d\n - Player X: %d Y: %d\n", 
        save_path, 
        sd->version, 
        pl->entity->coords->x, pl->entity->coords->y
    );

    debug_file(dbgOut, 1, " - Mobs:\n");
    for (int i = 0; i < count; i++) {
        debug_file(dbgOut, 0, " -- X: %d Y: %d.\n", mobs[i]->entity->coords->x, mobs[i]->entity->coords->y);
    }


}
#pragma endregion


