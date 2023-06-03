#include "controlers.h"


void start_game(int HEIGHT, int WIDTH) {

	closeMenu(g_renderstate->menus[0]->id);

	ALTURA_JOGO = HEIGHT;
	LARGURA_JOGO = WIDTH;
	
	g_gamestate->input_initialized = 1;
	g_gamestate->clock = 0;
	g_gamestate->block_clock = 1;
	g_gamestate->last_res = -1;
	g_gamestate->paused = FALSE;

	init_light_map(ALTURA_JOGO, LARGURA_JOGO);
	find_map = create_random_map(ALTURA_JOGO, LARGURA_JOGO, OFFSET_Y, OFFSET_X);

	for(int i = 0 ; i < ALTURA_JOGO ; i++)
		for(int j = 0 ; j < LARGURA_JOGO ; j++)
			map_footprint[i][j] = map[i][j];

	g_gamestate->mob_count = 3;
	g_gamestate->chest_count = 1;
	g_gamestate->projectile_count = 2;

	default_values();

	for (int i = 0; i < g_gamestate->mob_count ; i++) {
		addRandomItemToMob(g_gamestate->mobs[i]);
		addMobToMap(g_gamestate->mobs[i], map, LARGURA_JOGO, ALTURA_JOGO);

		map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
	}

	for(int i = 0 ; i < g_gamestate->chest_count ; i++)
	    addChestToMap(g_gamestate->chests[i], map, LARGURA_JOGO, ALTURA_JOGO);

	player_spawn(g_gamestate->player, map, ALTURA_JOGO, LARGURA_JOGO); 

}


void end_game() {

	// -- Gamestate

	// Free map
	for(int i = 0 ; i < ALTURA_JOGO ; i++) {

		if(map[i] == NULL)
			break;
		
		free(map[i]);
	}
	free(map);

	// Free map_footprint
	for(int i = 0 ; i < ALTURA_JOGO ; i++) {

		if(map_footprint[i] == NULL)
			break;
		
		free(map_footprint[i]);
	}
	free(map_footprint);

	// Free light_map
	for(int i = 0 ; i < ALTURA_JOGO ; i++) {

		if(visible[i] == NULL)
			break;
	
		free(visible[i]);
	}
	free(visible);

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
	
	// Free gamestate
	free(g_gamestate);


	// -- Renderstate

	// Free menus
	closeMenu(g_renderstate->menus[0]->id);

	// Free locales
	// Por adicionar !!!!!!!!!!!!!!

	// Free window
	endwin();

	// Free renderstate
	free(g_renderstate);


	// -- General

	// Free items
	// Por adicionar !!!!!!!!!!

}


void default_values() {

	// Default player values
		// Player Entity
	g_gamestate->player->entity->coords->x = 0;
	g_gamestate->player->entity->coords->y = 0;
    g_gamestate->player->entity->armor = 0;
		// Player Cheats
	g_gamestate->player->cheats->godmode = g_gamestate->player->cheats->vision = 0;
		// Player Item
	g_gamestate->player->item = get_random_item();
		// Player Values
    g_gamestate->player->level = 0;
    g_gamestate->player->kills = 0;
    g_gamestate->player->xp = 0;

	// Default projectile values
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

	// Default mob values
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
    g_gamestate->mobs[i]->hitChance = 3;
    g_gamestate->mobs[i]->hitCooldown = 1;
    g_gamestate->mobs[i]->lastHit = 0;
	}

	// Default chest values
	for(int i = 0 ; i < g_gamestate->chest_count ; i++) {
		// Chest Entity
	g_gamestate->chests[i]->entity->coords->x = 0;
	g_gamestate->chests[i]->entity->coords->y = 0;
	g_gamestate->chests[i]->entity->maxHealth = 12;
    g_gamestate->chests[i]->entity->health = 12;
    g_gamestate->chests[i]->entity->armor = 0;
    g_gamestate->chests[i]->entity->basedamage = 4;
	}


}