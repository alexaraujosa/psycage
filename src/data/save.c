#include "save.h"
#include <unistd.h>
#include <stdio.h>
#include "../render/render.h"
#include "../gameloop/gameloop.h"

void get_savepath(char *save_path, int num_save) {

    snprintf(save_path, PATH_MAX, "%s/saves/save%d.txt", ASSET_DIR, num_save);
    return;

}


int verify_Save(int num_save) {
    
    char save_path[PATH_MAX];
    get_savepath(save_path, num_save);

    if(access(save_path, F_OK) != -1)
        return 1;
    
    return 0;
}


int delete_Save(int num_save) {

    char save_path[PATH_MAX];
    get_savepath(save_path, num_save);

    if(verify_Save(num_save) == 1) {

        if(access(save_path, F_OK) != -1) {

            int verifier = remove(save_path);

            if(verifier == -1) {
                debug("Error deleting file %s.", save_path);
                return 0;
            }

            return 1;

        }

    } else {

        debug("Missing save file in %s.", save_path);
        return 0;

    }

    return 0;
}


void create_Save(int num_save) {

    char save_path[PATH_MAX];
    get_savepath(save_path, num_save);

    FILE *save = fopen(save_path, "w");

    if(save == NULL) {
        debug("Error while creating save file %s.", save_path);
        exit(0);
    }

    fseek(save, 0, SEEK_SET);

    fprintf(save, "#version=1");

    fseek(save, 1, SEEK_CUR);

    fprintf(save, "OLA");


    fclose(save);
    return;

}


