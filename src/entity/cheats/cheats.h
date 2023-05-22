#ifndef __RL_CHEATS_H
#define __RL_CHEATS_H

#include "../entity.h"
#include <ctype.h>

typedef struct cheats {
    int godmode;
    int vision;
} CHEATS, *Cheats;

/* FUNCTION SIGNATURES */
Cheats defaultCheats();
void destroyCheats(Cheats cheats);
// void godmode_code_checker(int key);
void godmode_Health();
// void vision_code_checker(int key);
void vision_Radius();

#endif