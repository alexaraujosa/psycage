#include "cheats.h"
#include "../../gameloop/gameloop.h"


static char godmode_code[] = "GODMODE";
static int godmode_code_index = 0;

void godmode_code_checker(int key) {

	if(toupper(key) == godmode_code[godmode_code_index]) {

		godmode_code_index++;

        if(godmode_code_index == (int) strlen(godmode_code))
            g_gamestate->player->cheats = (g_gamestate->player->cheats == 0) ? 1 : 0;

	} else {

        godmode_code_index = 0;

    }


}

int is_in_godmode() {
    return g_gamestate->player->cheats;
}