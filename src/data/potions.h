/***************************************************************
 * POTIONS MODULE                                              *
 *                                                             *
 * This module handles the usage and placement of potions on   *
 * the map.                                                    *
 *                                                             *
 ***************************************************************/

#ifndef __RL_DATA_POTIONS_H
#define __RL_DATA_POTIONS_H

#include "common.h"
#include "../entity/ai/ai.h"
#include "../gameloop/gameloop.h"


/* FUNCTION SIGNATURES */
void init_potions_clock();
void create_potion();
void use_random_potion();
void remove_potion_strength_effect();
void potion_checker();

#endif