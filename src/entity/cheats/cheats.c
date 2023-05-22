#include "cheats.h"
#include "../../gameloop/gameloop.h"


static char godmode_code[] = "GODMODE";
static char vision_code[] = "VISION";
static int godmode_code_index = 0, vision_code_index = 0;
static int vidaAntes = 0, visaoAntes = 0;

Cheats defaultCheats() {
    Cheats cheats = (Cheats)malloc(sizeof(CHEATS));
    if (cheats == NULL) return NULL;

    cheats->godmode = 0;
    cheats->vision = 0;

    return cheats;;
}

void destroyCheats(Cheats cheats) {
    free(cheats);
    return;
}


// void godmode_code_checker(int key) {

// 	if(toupper(key) == godmode_code[godmode_code_index]) {

// 		godmode_code_index++;

//         if(godmode_code_index == (int)strlen(godmode_code)) {
//             g_gamestate->player->cheats = (g_gamestate->player->cheats == 0) ? 1 : 0;
//             godmode_Health();
//         }
// 	} else {

//         godmode_code_index = 0;

//     }


// }

void godmode_Health() {

    if (g_gamestate->player->cheats->godmode == 1) {
        vidaAntes = g_gamestate->player->entity->health;
    }

    g_gamestate->player->entity->health = (g_gamestate->player->cheats->godmode == 0) ? vidaAntes : 999999999999;

}

// void vision_code_checker(int key) {

// 	if(toupper(key) == vision_code[vision_code_index]) {

// 		vision_code_index++;

//         if(vision_code_index == (int)strlen(vision_code)) {
//             g_gamestate->player->cheats->vision = (g_gamestate->player->cheats->vision == 0) ? 1 : 0;
//             vision_Radius();
//         }
// 	} else {

//         vision_code_index = 0;

//     }


// }

void vision_Radius() {

    if (g_gamestate->player->cheats->vision == 1) {
        visaoAntes = g_gamestate->player->radius;
    }

    g_gamestate->player->radius = (g_gamestate->player->cheats->vision == 0) ? visaoAntes : 100;

}