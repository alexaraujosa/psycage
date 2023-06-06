#include "projectile.h"
#include "../../gameloop/gameloop.h"
#include <string.h>

#define RAIO_TRAP 3
#define RAIO_MOLOTOV 2
#define DISTANCIA_ATIRAR_TRAP 4
#define DISTANCIA_ATIRAR_MOLOTOV 7
#define TEMPO_TRAP 10
#define TEMPO_MOLOTOV 15

Clock clock_trap = NULL;
Clock clock_molotov = NULL;

// static int raioAnterior = 0;
static int contagem_molotov = 0, contagem_trap = 0;

Projectile defaultProjectile() {
    Projectile projectile = (Projectile)malloc(sizeof(PROJECTILE));
    if (projectile == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;

    projectile->entity = entity;
    projectile->dx = 0;
    projectile->dy = 0;

    return projectile;
}

void destroyProjectile(Projectile projectile) {
    destroyEntity(projectile->entity);
    free(projectile);
}

void init_grenades_clock() {
    clock_trap = clock_molotov = defaultClock();
    clock_trap->maxTicks *= TEMPO_TRAP;
    clock_trap->blocked = 1;
    addClock(clock_trap);
    clock_molotov->maxTicks *= TEMPO_MOLOTOV;
    clock_molotov->blocked = 1;
    addClock(clock_molotov);

    return;
}

void move_trap(int dx, int dy) {


        if(is_passable(g_gamestate->projectiles[1]->entity->coords->x + dx, g_gamestate->projectiles[1]->entity->coords->y + dy) && contagem_trap < DISTANCIA_ATIRAR_TRAP) {
		    g_gamestate->projectiles[1]->entity->coords->x += dx;
		    g_gamestate->projectiles[1]->entity->coords->y += dy;
            contagem_trap++;
        }
        else
            deploy_trap();

    return;
}

void deploy_trap() {

    for(int y = RAIO_TRAP ; y >= -RAIO_TRAP ; y--)
        for(int x = -RAIO_TRAP ; x <= RAIO_TRAP; x++) {
            if(g_gamestate->projectiles[1]->entity->coords->y + y >= ALTURA_JOGO ||
               g_gamestate->projectiles[1]->entity->coords->y + y <= 0 ||
               g_gamestate->projectiles[1]->entity->coords->x + x >= LARGURA_JOGO ||
               g_gamestate->projectiles[1]->entity->coords->x + x <= 0
            )
            continue;

            if(x*x + y*y <= RAIO_TRAP*RAIO_TRAP && 
               map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] != 1 && 
               map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] != 3
            ) 
            map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] = 8;
        }

    map[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x] = 8;

    for(int i = 0 ; i < g_gamestate->mob_begin ; i++){
        if(map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] == 8 && 
           g_gamestate->mobs[i]->entity->coords->y != 0 && 
           g_gamestate->mobs[i]->entity->coords->x != 5
        ) {
            g_gamestate->mobs[i]->hasAI = FALSE;
            map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
        }
    }

    for(int i = 0 ; i < g_gamestate->chest_count ; i++)
        if(map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] == 8)
            map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] = 5;
    
    clock_trap->blocked = 0;
    g_gamestate->player->trap = 0;

    return;
}

void remove_trap() {

    for(int y = RAIO_TRAP ; y >= -RAIO_TRAP ; y--){
        for(int x = -RAIO_TRAP ; x <= RAIO_TRAP; x++){
            if(g_gamestate->projectiles[1]->entity->coords->y + y >= ALTURA_JOGO ||
               g_gamestate->projectiles[1]->entity->coords->y + y <= 0 ||
               g_gamestate->projectiles[1]->entity->coords->x + x >= LARGURA_JOGO ||
               g_gamestate->projectiles[1]->entity->coords->x + x <= 0
            )
            continue;

            if(x*x + y*y <= RAIO_TRAP*RAIO_TRAP) 
                map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] = map_footprint[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x];
        }
    }
    map[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x] = map_footprint[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x];

    for(int i = 0 ; i < g_gamestate->mob_begin ; i++) {
        if(g_gamestate->mobs[i]->entity->coords->y != 0 && g_gamestate->mobs[i]->entity->coords->x != 5) {
            map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
            g_gamestate->mobs[i]->hasAI = TRUE;
        }
    }

    for(int i = 0 ; i < g_gamestate->chest_count ; i++)
        map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] = 5;

    contagem_trap = 0;
    clock_trap->blocked = 1;
    clock_trap->ticks = 0;
    g_gamestate->projectiles[1]->entity->coords->y = g_gamestate->projectiles[1]->entity->coords->x = 0;

    return;
}

void trap_checker() {

    if(clock_trap->ticks == TICKS_PER_SECOND*TEMPO_TRAP - 1)
        remove_trap();

    return;
}



void deploy_molotov() {

    for(int y = RAIO_MOLOTOV ; y >= -RAIO_MOLOTOV ; y--)
        for(int x = -RAIO_MOLOTOV ; x <= RAIO_MOLOTOV; x++) {
            if(g_gamestate->projectiles[2]->entity->coords->y + y >= ALTURA_JOGO ||
               g_gamestate->projectiles[2]->entity->coords->y + y <= 0 ||
               g_gamestate->projectiles[2]->entity->coords->x + x >= LARGURA_JOGO ||
               g_gamestate->projectiles[2]->entity->coords->x + x <= 0
            )
            continue;

            if(x*x + y*y <= RAIO_MOLOTOV*RAIO_MOLOTOV && 
               map[g_gamestate->projectiles[2]->entity->coords->y + y][g_gamestate->projectiles[2]->entity->coords->x + x] != 1 && 
               map[g_gamestate->projectiles[2]->entity->coords->y + y][g_gamestate->projectiles[2]->entity->coords->x + x] != 3
            ) 
            map[g_gamestate->projectiles[2]->entity->coords->y + y][g_gamestate->projectiles[2]->entity->coords->x + x] = 9;
        }

    map[g_gamestate->projectiles[2]->entity->coords->y][g_gamestate->projectiles[2]->entity->coords->x] = 9;
        

    for(int i = 0 ; i < g_gamestate->chest_count ; i++)
        if(map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] == 9)
            map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] = 5;
    
    clock_molotov->blocked = 0;
    g_gamestate->player->molotov = 0;

    return;
}

void move_molotov(int dx, int dy) {

        if(is_passable(g_gamestate->projectiles[2]->entity->coords->x + dx, g_gamestate->projectiles[2]->entity->coords->y + dy) &&
         contagem_molotov < DISTANCIA_ATIRAR_MOLOTOV) 
        {
		    g_gamestate->projectiles[2]->entity->coords->x += dx;
		    g_gamestate->projectiles[2]->entity->coords->y += dy;
            contagem_molotov++;
        }
        else
            deploy_molotov();

    return;
}

void remove_molotov() {

    for(int y = RAIO_MOLOTOV ; y >= -RAIO_MOLOTOV ; y--){
        for(int x = -RAIO_MOLOTOV ; x <= RAIO_MOLOTOV; x++){
            if(g_gamestate->projectiles[2]->entity->coords->y + y >= ALTURA_JOGO ||
               g_gamestate->projectiles[2]->entity->coords->y + y <= 0 ||
               g_gamestate->projectiles[2]->entity->coords->x + x >= LARGURA_JOGO ||
               g_gamestate->projectiles[2]->entity->coords->x + x <= 0
            )
            continue;

            if(x*x + y*y <= RAIO_MOLOTOV*RAIO_MOLOTOV) 
                map[g_gamestate->projectiles[2]->entity->coords->y + y][g_gamestate->projectiles[2]->entity->coords->x + x] = map_footprint[g_gamestate->projectiles[2]->entity->coords->y + y][g_gamestate->projectiles[2]->entity->coords->x + x];
        }
    }
    map[g_gamestate->projectiles[2]->entity->coords->y][g_gamestate->projectiles[2]->entity->coords->x] = map_footprint[g_gamestate->projectiles[2]->entity->coords->y][g_gamestate->projectiles[2]->entity->coords->x];

    for(int i = 0 ; i < g_gamestate->mob_count ; i++)
        map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;

    for(int i = 0 ; i < g_gamestate->chest_count ; i++)
        map[g_gamestate->chests[i]->entity->coords->y][g_gamestate->chests[i]->entity->coords->x] = 5;

    contagem_molotov = 0;
    clock_molotov->blocked = 1;
    clock_molotov->ticks = 0;
    g_gamestate->projectiles[2]->entity->coords->y = g_gamestate->projectiles[2]->entity->coords->x = 0;
    
    return;
}

void molotov_checker() {

    if(clock_trap->ticks == TICKS_PER_SECOND*TEMPO_TRAP - 1)
        remove_molotov();

    return;
}

void molotov_mob_checker() {
    
    for(int i = 0 ; i < g_gamestate->mob_begin ; i++) {
        if(
            map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] == 9 && 
            g_gamestate->mobs[i]->entity->health > 0 && 
            g_gamestate->mobs[i]->entity->coords->y != 0 &&
            g_gamestate->mobs[i]->entity->coords->x != 5
            )
            damageMob(g_gamestate->mobs[i], 1);
    }

    return;
}
void molotov_player_checker() {
    if(map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 9 && g_gamestate->player->entity->health > 0)
        damageEntity(g_gamestate->player->entity, 1);

    return;
}

void reset_contagem() {
    contagem_molotov = 0;
    contagem_trap = 0;

    return;
}