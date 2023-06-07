#include "ai.h"
#include "../player/player.h"

#define NUM_OBSTACLES 5
static int obstacles[NUM_OBSTACLES] = { 1, 3, 5, 6, 7 };

#define NUM_ITEMS 1
static char *mob_itens[NUM_ITEMS] = {"Broken_Sword"};

Mob defaultMob() {
    Mob mob = (Mob)malloc(sizeof(MOB));
    if (mob == NULL) return NULL;

    mob->entity = defaultEntity();
    
    if (g_gamestate != NULL && g_gamestate->input_initialized) {
        for (int i = 0; i < g_gamestate->player->level; i++) {
            mob->entity->basedamage += 4;
            mob->entity->maxHealth += 6;
            mob->entity->health += 6;
        }
    }

    mob->item = get_item_by_id("0000");

    mob->moveCooldown = 10;
    mob->lastMove = 0;

    if (g_gamestate != NULL && g_gamestate->input_initialized) {
        for (int i = 0; i < (g_gamestate->player->level / 2); i++) {
            if (mob->moveCooldown == 1) break;
            mob->moveCooldown -= 1;
        }
    }

    mob->hasAI = TRUE;

    // mob->hitChance = 3;
    mob->hitCooldown = 1;
    mob->lastHit = 0;

    return mob;
}

void destroyMob(Mob mob) {
    destroyEntity(mob->entity);

    free(mob);
}

int attemptDamagePlayer(Mob mob) {
    // if (!mob->hasAI) {
    //     debug_file(dbgOut, 2, "Mob (X %d, Y %d) cannot damage because it has no AI\n", mob->entity->coords->x, mob->entity->coords->y);
    //     return 1;
    // }

    debug_file(dbgOut, 2, "Attempting to damage player, mob (X %d, Y %d).\n", mob->entity->coords->x, mob->entity->coords->y);

    int distX = g_gamestate->player->entity->coords->x - mob->entity->coords->x;
    int distY = g_gamestate->player->entity->coords->y - mob->entity->coords->y;
    double distance = sqrt(distX * distX + distY * distY);

    mob->lastHit++;
    
    if (distance <= sqrt(2)) {
        if (mob->lastHit <= mob->hitCooldown) {
            debug_file(dbgOut, 2, "- On hit cooldown (%d / %d). Skipping\n", mob->lastHit, mob->hitCooldown);
            return 1;
        }

        int dmg = mob->entity->basedamage + mob->item->damage;
        damageEntity(g_gamestate->player->entity, dmg);
        mob->lastHit = 0;

        debug_file(dbgOut, 2, "- Damaged player with %d damage.\n", dmg);

        // int chance = rand() % mob->hitChance + 1;

        // if (chance == mob->hitChance / 2) {
        //     int dmg = mob->entity->basedamage + mob->item->damage;

        //     debug_file(dbgOut, 2, "- Chance hit. Damaging player with %d damage points.\n", dmg);
        //     debug_file(dbgOut, 2, "- Damage components: %d %d.\n", mob->entity->basedamage, mob->item->damage);

        //     damageEntity(g_gamestate->player->entity, dmg);
        //     mob->lastHit = 0;
        //     return 0;
        // } else {
        //     debug_file(dbgOut, 2, "- Chance miss.\n");
        //     return 1;
        // }

        return 0;
    }

    debug_file(dbgOut, 2, "- Player is too far away. No damage can be done (%f > sqrt(2)).\n", distance);

    return 1;
}

int trackPlayer(Coords playerCoords, Mob mob, int** map, int width, int height) {
    debug_file(dbgOut, 2, "Calculating path for mob (X %d, Y %d).\n", mob->entity->coords->x, mob->entity->coords->y);

    debug_file(dbgOut, 2, "- Resetting mob tile.\n");
    map[mob->entity->coords->y][mob->entity->coords->x] = map_footprint[mob->entity->coords->y][mob->entity->coords->x];

    debug_file(dbgOut, 2, "- Calculating path.\n");
	As_Node path = pathfind(
		height, 
		width, 
		map, 
		playerCoords, 
		mob->entity->coords, 
		obstacles,
		NUM_OBSTACLES
	);

    if (path != NULL) {
        int path_cell_count = pf_get_path_cell_count(path);
        Coords* path_cells = pf_reconstruct_path(path, path_cell_count);

        if (path_cell_count > 2) { // Points not adjacent cardinally
            // asylum[mob->entity->coords->y][mob->entity->coords->x] = map_footprint[mob->entity->coords->y][mob->entity->coords->x];
            debug_file(
                dbgOut, 2, 
                "- Path found. Moving mob from (X %d, Y %d) to (X %d, Y %d).\n", 
                mob->entity->coords->x, mob->entity->coords->y,
                path_cells[1]->x, path_cells[1]->y
            );
            
            mob->entity->coords = path_cells[1];
            map[mob->entity->coords->y][mob->entity->coords->x] = 5;

            return 0;
        } else {
            debug_file(dbgOut, 2, "- Mob is adjacent to player. Attempting to damage.\n");

            map[mob->entity->coords->y][mob->entity->coords->x] = 5;
            return attemptDamagePlayer(mob);
        }

        map[mob->entity->coords->y][mob->entity->coords->x] = 5;

        debug_file(dbgOut, 2, "Mob is already adjacent to player. Skipping.\n");

        return 1;
    }

    debug_file(dbgOut, 2, "- Path not found.\n");

    return 1;
}

int attemptMoveMob(Coords playerCoords, Mob mob, int** map, int width, int height) {
    if (!mob->hasAI) return 1;
    
    mob->lastMove++;

    if (mob->lastMove >= mob->moveCooldown) {
        debug_file(dbgOut, 2, "Attempting to move mob (X %d, Y %d).\n", mob->entity->coords->x, mob->entity->coords->y);

        mob->lastMove = 0;
        return trackPlayer(playerCoords, mob, map, width, height);
    }

    return 1;
}

int _isObstacle(int cell) {
    for (int i = 0; i < NUM_OBSTACLES; i++) {
        if (cell == obstacles[i]) return 1;
    }

    return 0;
}

int addMobToMap(Mob mob, int** map, int width, int height) {
    int cellX = -1, cellY = -1;
    
    while ((cellX == -1 && cellY == -1) || _isObstacle(map[cellY][cellX])) {
        cellX = (int)floor(((double) rand() / RAND_MAX) * (width - 1));
        cellY = (int)floor(((double) rand() / RAND_MAX) * (height - 1));
        // printf(
        //     "X: %d | Y: %d | %d %d %f %f\n", 
        //     cellX, 
        //     cellY, 
        //     width - 1, 
        //     height - 1,
        //     ((double) rand() / RAND_MAX) * (width - 1),
        //     ((double) rand() / RAND_MAX) * (height - 1)
        // );
    }

    mob->entity->coords->x = cellX;
    mob->entity->coords->y = cellY;

    return 0;
}

void addRandomItemToMob(Mob mob) {
    int index = rand() % NUM_ITEMS;
    
    mob->item = get_item_by_name(mob_itens[index]);
}

void damageMob(Mob mob, int hp) {
    if(mob->entity->health > 0)
        damageEntity(mob->entity, hp);
    else {
        killMob(mob);
    }
    
    return;
}

void MobAliveChecker(Mob mob) {
    if(
        (
            mob->entity->coords->y == g_gamestate->projectiles[0]->entity->coords->y+1 
            && mob->entity->coords->x == g_gamestate->projectiles[0]->entity->coords->x
        )  || (
            mob->entity->coords->y == g_gamestate->projectiles[0]->entity->coords->y-1 
            && mob->entity->coords->x == g_gamestate->projectiles[0]->entity->coords->x
        ) || (
            mob->entity->coords->y == g_gamestate->projectiles[0]->entity->coords->y 
            && mob->entity->coords->x == g_gamestate->projectiles[0]->entity->coords->x+1
        ) || (
            mob->entity->coords->y == g_gamestate->projectiles[0]->entity->coords->y 
            && mob->entity->coords->x == g_gamestate->projectiles[0]->entity->coords->x-1
        )
    ) damageMob(mob, g_gamestate->player->entity->basedamage + g_gamestate->player->item->damage);
}

void killMob(Mob mob) {

    mob->hasAI = FALSE;
    map[mob->entity->coords->y][mob->entity->coords->x] = map_footprint[mob->entity->coords->y][mob->entity->coords->x];
    mob->entity->coords->y = 0;
    mob->entity->coords->x = 5;
    killCount(g_gamestate->player, mob->entity);
    levelUp(mob);
    g_gamestate->mob_count--;


    return;
}

void levelUp(Mob mob) {
    if (g_gamestate->player->level <= 99) {
        if (g_gamestate->player->kills % 5 == 0) {
            g_gamestate->player->level += 1;

            for (int i = 0; i < g_gamestate->mob_begin; i++) {
                if (
                    g_gamestate->mobs[i]->entity->coords->y != 0 
                    && g_gamestate->mobs[i]->entity->coords->x != 5
                    && g_gamestate->mobs[i]->entity->health != 0
                ) {
                    mob->entity->basedamage += 4;
                    mob->entity->maxHealth += 6;
                    mob->entity->health += 6;
                } 
            }

			   if(strcmp(g_gamestate->player->item->id, "0007") == 0 || strcmp(g_gamestate->player->item->id, "0008") == 0 || strcmp(g_gamestate->player->item->id, "0009") == 0 
			   || strcmp(g_gamestate->player->item->id, "0016") == 0 || strcmp(g_gamestate->player->item->id, "0018") == 0){
                 if(g_gamestate->player->class == Priest){
						 g_gamestate->player->entity->basedamage += 2;
                         g_gamestate->player->entity->maxHealth += 4;
				 }
                 if(g_gamestate->player->class == Mercenary){
						 g_gamestate->player->entity->basedamage += 3;
                         g_gamestate->player->entity->maxHealth += 3;
				 }
                 if(g_gamestate->player->class == Detective){
						 g_gamestate->player->entity->basedamage += 4;
                         g_gamestate->player->entity->maxHealth += 2;
				 }
			   }
               if(strcmp(g_gamestate->player->item->id, "0001") == 0 || strcmp(g_gamestate->player->item->id, "0002") == 0 || strcmp(g_gamestate->player->item->id, "0003") == 0 
			   || strcmp(g_gamestate->player->item->id, "0010") == 0 || strcmp(g_gamestate->player->item->id, "0014") == 0){
                 if(g_gamestate->player->class == Priest){
					     g_gamestate->player->entity->basedamage += 4;
                         g_gamestate->player->entity->maxHealth += 4;
				 }
                 if(g_gamestate->player->class == Mercenary){
						 g_gamestate->player->entity->basedamage += 6;
                         g_gamestate->player->entity->maxHealth += 3;
				 }
                 if(g_gamestate->player->class == Detective){
						 g_gamestate->player->entity->basedamage += 8;
                         g_gamestate->player->entity->maxHealth += 2;
				 }
			   }
               if(strcmp(g_gamestate->player->item->id, "0004") == 0 || strcmp(g_gamestate->player->item->id, "0005") == 0 || strcmp(g_gamestate->player->item->id, "0006") == 0 
			   || strcmp(g_gamestate->player->item->id, "0011") == 0 || strcmp(g_gamestate->player->item->id, "0012") == 0 || strcmp(g_gamestate->player->item->id, "0013") == 0 
			   || strcmp(g_gamestate->player->item->id, "0015") == 0 || strcmp(g_gamestate->player->item->id, "0017") == 0){
                 if(g_gamestate->player->class == Priest){
                		 g_gamestate->player->entity->basedamage += 2;
                         g_gamestate->player->entity->maxHealth += 8;
				 }
                 if(g_gamestate->player->class == Mercenary){
						 g_gamestate->player->entity->basedamage += 3;
                         g_gamestate->player->entity->maxHealth += 6;
				 }
                 if(g_gamestate->player->class == Detective){
						 g_gamestate->player->entity->basedamage += 4;
                         g_gamestate->player->entity->maxHealth += 4;
				 }
			   }

			}
            g_gamestate->player->entity->health = g_gamestate->player->entity->maxHealth;

    }
}