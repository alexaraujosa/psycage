#ifndef __RL_DATA_SAVE_H
#define __RL_DATA_SAVE_H

#include <stdio.h>
#include "common.h"
#include "util/string.h"
#include "util/hashmap.h"
#include "../gameloop/gameloop.h"
#include "debug.h"

int verify_Save(int num_save);
int delete_Save(int num_save);
int create_Save(int num_save);


#endif