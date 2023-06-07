#include "controlers.h"

void addCandleToMap() {
	int cellX = -1, cellY = -1;
    
    while ((cellX == -1 && cellY == -1) || _isObstacle(map[cellY][cellX])) {
        cellX = (int)floor(((double) rand() / RAND_MAX) * (LARGURA_JOGO - 1));
        cellY = (int)floor(((double) rand() / RAND_MAX) * (ALTURA_JOGO - 1));
    }

    map[cellY][cellX] = 6;
}

void start_game() {
	if (g_renderstate->activeMenus > 0) closeMenu(g_renderstate->menus[0]->id);
	reset_contagem();
	
	// g_gamestate->input_initialized = 1;
	// g_gamestate->clock = 0;
	// g_gamestate->block_clock = 1;
	// g_gamestate->last_res = -1;
	g_gamestate->paused = FALSE;

	// init_light_map(ALTURA_JOGO, LARGURA_JOGO);
	for (int i = 0 ; i < ALTURA_JOGO ; i++)
        for (int j = 0 ; j < LARGURA_JOGO ; j++)
            visible[i][j] = 0;

	find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X);

	for (int i = 0 ; i < ALTURA_JOGO ; i++)
		for (int j = 0 ; j < LARGURA_JOGO ; j++)
			map_footprint[i][j] = map[i][j];

	g_gamestate->mob_count = 3;
	g_gamestate->chest_count = 1;
	g_gamestate->projectile_count = 3;
	g_gamestate->potion_strength = 0;

	default_values();

	for (int i = 0; i < g_gamestate->mob_count ; i++) {
		addRandomItemToMob(g_gamestate->mobs[i]);
		addMobToMap(g_gamestate->mobs[i], map, LARGURA_JOGO, ALTURA_JOGO);

		map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
	}

	for (int i = g_gamestate->mob_count ; i < g_gamestate->mob_begin ; i++) {
		if (g_gamestate->mobs[i] != NULL) free(g_gamestate->mobs[i]);
	}

	g_gamestate->mob_begin = g_gamestate->mob_count;

	for (int i = 0 ; i < g_gamestate->chest_count ; i++)
	    addChestToMap(g_gamestate->chests[i], map, LARGURA_JOGO, ALTURA_JOGO);

	player_spawn(g_gamestate->player, map, ALTURA_JOGO, LARGURA_JOGO); 
	create_potion();

	for (int i = 0; i < CANDLES_NUM; i++) {
		addCandleToMap();
	}

	g_gamestate->valid_state = TRUE;
	g_gamestate->input_initialized = TRUE;

	return;
}

void continue_game() {
	if (g_gamestate->projectiles[1]->entity->coords->y != 0)	remove_trap();
    if (g_gamestate->projectiles[2]->entity->coords->y != 0)	remove_molotov();
	resetAllClocks();

	for (int y = 0; y < ALTURA_JOGO; y++) {	
		if (map == NULL) break;	
		free(map[y]);
	}
	free(map);


	for (int y = 0 ; y < ALTURA_JOGO ; y++)
		for (int x = 0 ; x < LARGURA_JOGO ; x++)
			visible[y][x] = 0;


	find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X); 
	
	while (!valid_map(ALTURA_JOGO, LARGURA_JOGO)) {
		for (int y = 0; y < ALTURA_JOGO-1; y++) {
			if (map == NULL) break;
        		
			free(map[y]);
		}

		free(map);

		find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X); 
	}
	
	for (int i = 0; i < ALTURA_JOGO; i++) {
		for (int j = 0; j < LARGURA_JOGO; j++) {
			map_footprint[i][j] = map[i][j];
		}
	}

	g_gamestate->mob_count = g_gamestate->mob_begin = (rand() % 6) + 7;

	for(int i = 0 ; i < g_gamestate->mob_begin ; i++) {
		g_gamestate->mobs[i] = defaultMob();
		addRandomItemToMob(g_gamestate->mobs[i]);
		addMobToMap(g_gamestate->mobs[i], map, LARGURA_JOGO, ALTURA_JOGO);

		map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
	}

	player_spawn(g_gamestate->player, map, ALTURA_JOGO, LARGURA_JOGO); 

	g_gamestate->chest_count = 1;
	for (int i = 0; i < g_gamestate->chest_count; i++) {
	    addChestToMap(g_gamestate->chests[i], map, LARGURA_JOGO, ALTURA_JOGO);
	    map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] = 5;
	}

	g_gamestate->potion_strength = 0;
	create_potion();

	for (int i = 0; i < CANDLES_NUM; i++) {
		addCandleToMap();
	}

	// if (is_player_insane(g_gamestate->player)) g_
	restore_sanity(g_gamestate->player, 200);

	return;
}


void end_game() {
	if (!g_gamestate->valid_state) return;
	if (!g_gamestate->input_initialized) goto end_game_final;

	// -- Gamestate

	// Free map
	for(int i = 0 ; i < ALTURA_JOGO ; i++) {

		if(map[i] == NULL)
			continue;
		
		free(map[i]);
	}
	free(map);

	// Free player
	destroyPlayer(g_gamestate->player);
	
	// Free projectiles
	for(int i = 0 ; i < g_gamestate->projectile_count ; i++)
		destroyProjectile(g_gamestate->projectiles[i]);
	
	// Free mobs
	for(int i = 0 ; i < g_gamestate->mob_count ; i++)
		destroyMob(g_gamestate->mobs[i]);
	
	// Free chests
	for(int i = 0 ; i < g_gamestate->chest_count ; i++)
		destroyChest(g_gamestate->chests[i]);

	end_game_final: {
		// Free gamestate
		free(g_gamestate);


		// -- Renderstate

		// Free menus
		if (g_renderstate->activeMenus > 0) closeMenu(g_renderstate->menus[0]->id);

		// Free window
		endwin();

		// Free renderstate
		free(g_renderstate);

		g_gamestate->valid_state = FALSE;
		EXIT = TRUE;

		return;
	}
}

void reset_player_values() {
	// Player Entity
	g_gamestate->player->entity->coords->x = 0;
	g_gamestate->player->entity->coords->y = 0;
    g_gamestate->player->entity->armor = 0;
	// Player Cheats
	g_gamestate->player->cheats->godmode = g_gamestate->player->cheats->vision = 0;
	// Player Item
	g_gamestate->player->item = get_item_by_id("0010");
	// Player Values
    g_gamestate->player->level = 0;
    g_gamestate->player->kills = 0;
	g_gamestate->player->sanity = 100;
	g_gamestate->player->candle_fuel = 5;
	g_gamestate->player->molotov = 0;
	g_gamestate->player->trap = 0;

	return;
}

void reset_projectiles_values() {
	for(int i = 0 ; i < g_gamestate->projectile_count ; i++) {
		// Projectile Entity
		g_gamestate->projectiles[i]->entity->coords->x = 0;
		g_gamestate->projectiles[i]->entity->coords->y = 0;
		g_gamestate->projectiles[i]->entity->maxHealth = 12;
    	g_gamestate->projectiles[i]->entity->health = 12;
    	g_gamestate->projectiles[i]->entity->armor = 0;
    	g_gamestate->projectiles[i]->entity->basedamage = 4;

		// Projectile Values
		g_gamestate->projectiles[i]->dx = 0;
		g_gamestate->projectiles[i]->dy = 0;
	}

	return;
}

void reset_mobs_values() {
	for(int i = 0 ; i < g_gamestate->mob_count ; i++) {
		// Mob Entity
		g_gamestate->mobs[i]->entity->coords->x = 0;
		g_gamestate->mobs[i]->entity->coords->y = 0;
		g_gamestate->mobs[i]->entity->maxHealth = 12;
    	g_gamestate->mobs[i]->entity->health = 12;
    	g_gamestate->mobs[i]->entity->armor = 0;
    	g_gamestate->mobs[i]->entity->basedamage = 4;
		// Mob Item
    	g_gamestate->mobs[i]->item = get_item_by_id("0000");
		// Mob Values
    	g_gamestate->mobs[i]->moveCooldown = 10;
    	g_gamestate->mobs[i]->lastMove = 0;
    	g_gamestate->mobs[i]->hasAI = TRUE;
    	// g_gamestate->mobs[i]->hitChance = 3;
    	g_gamestate->mobs[i]->hitCooldown = 1;
    	g_gamestate->mobs[i]->lastHit = 0;
	}

	return;
}

void reset_chests_values() {
	for(int i = 0 ; i < g_gamestate->chest_count ; i++) {
		// Chest Entity
		g_gamestate->chests[i]->entity->coords->x = 0;
		g_gamestate->chests[i]->entity->coords->y = 0;
		g_gamestate->chests[i]->entity->maxHealth = 12;
    	g_gamestate->chests[i]->entity->health = 12;
    	g_gamestate->chests[i]->entity->armor = 0;
    	g_gamestate->chests[i]->entity->basedamage = 4;
	}

	return;
}

void default_values() {
	reset_player_values();
	reset_projectiles_values();
	reset_mobs_values();
	reset_chests_values();

	return;
}