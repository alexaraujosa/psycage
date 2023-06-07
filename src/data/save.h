/***************************************************************
 * SAVE MODULE                                                 *
 *                                                             *
 * This module handles the creation, loading and deltion of    *
 * save data.                                                  *
 *                                                             *
 * FAIR WARNING: This shit is more unstable than my own head,  *
 * so pray if you have to debug anything in here, cause you    *
 * won't. You'll just cry.                                     *
 ***************************************************************/

#ifndef __RL_DATA_SAVE_H
#define __RL_DATA_SAVE_H

#include <stdio.h>
#include "common.h"
#include "util/string.h"
#include "util/hashmap.h"
#include "../gameloop/gameloop.h"
#include "debug.h"

struct save_data {
    unsigned short version;
    char* test;
};

/* FUNCTION SIGNATURES */
int verify_Save(int num_save);
int delete_Save(int num_save);
int create_Save(int num_save);
int load_save(int num_save);


#endif