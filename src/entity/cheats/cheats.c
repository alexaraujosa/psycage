#include "cheats.h"
#include "../../gameloop/gameloop.h"


static char godmode_code[] = "GODMODE";
static int godmode_code_index = 0;
static int vidaAntes = 0;

void godmode_code_checker(int key) {

	if(toupper(key) == godmode_code[godmode_code_index]) {

		godmode_code_index++;

        if(godmode_code_index == (int) strlen(godmode_code)) {
            g_gamestate->player->cheats = (g_gamestate->player->cheats == 0) ? 1 : 0;
            godmode_Health();
        }
	} else {

        godmode_code_index = 0;

    }


}

void godmode_Health() {

    if (g_gamestate->player->cheats == 1) {
        vidaAntes = g_gamestate->player->entity->health;
    }

    g_gamestate->player->entity->health = (g_gamestate->player->cheats == 0) ? vidaAntes : 999999999999;

}