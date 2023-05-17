#include "save.h"
#include <unistd.h>
#include <stdio.h>
#include "../render/render.h"
#include "../gameloop/gameloop.h"

extern FILE* dbgOut;

int get_savepath(char *save_path, int num_save) {
    return snprintf(save_path, PATH_MAX, "%s/saves/save%d.txt", ASSET_DIR, num_save);
}

int verify_Save(int num_save) {
    
    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) return 1;

    if(access(save_path, F_OK) != -1) return 0;
    
    return 1;
}


int delete_Save(int num_save) {

    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) return 0;

    if(verify_Save(num_save) == 0) {

        // if(access(save_path, F_OK) != -1) {

            int verifier = remove(save_path);

            if(verifier == -1) {
                debug_file(dbgOut, "Cannot delete save file %s: unknown error.", save_path);
                return 0;
            }

            return 1;

        // }

    } else {
        debug_file(dbgOut, "Cannot delete save file %s: missing save file in %s.", save_path, save_path);
        return 0;
    }

    return 0;
}


int create_Save(int num_save) {

    char save_path[PATH_MAX];
    if (get_savepath(save_path, num_save) != 0) return 1;

    FILE *save = fopen(save_path, "w");

    if(save == NULL) {
        debug_file(dbgOut, "Error while creating save file %s.", save_path);
        return 1;
        // exit(0);
    }

    fseek(save, 0, SEEK_SET);

    fprintf(save, "#version=1");

    fseek(save, 1, SEEK_CUR);

    fprintf(save, "OLA");


    fclose(save);

    return 0;
}


