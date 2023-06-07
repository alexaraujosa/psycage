/***************************************************************
 * CONTROLERS MODULE                                           *
 *                                                             *
 * This module handles the control flux of the game itself.    *
 *                                                             *
 * NOTE: At least this module is controlling something, cause  *
 * I definitely aren't.                                        *
 ***************************************************************/

#ifndef __RL_CONTROLERS_H
#define __RL_CONTROLERS_H

#include "gameloop.h"

#define CANDLES_NUM 10

void start_game();
void continue_game();
void end_game();
void default_values();
void reset_player_values();
void reset_projectiles_values();
void reset_mobs_values();
void reset_chests_values();

#endif