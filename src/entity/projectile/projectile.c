#include "projectile.h"
#include "../../gameloop/gameloop.h"
#include <string.h>

#define RAIO_SMOKE 3
#define DISTANCIA_ATIRAR_SMOKE 4

static int raioAnterior = 0;
static int contagem = 0;

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

void moveSmoke(int dx, int dy) {


        if(is_passable(g_gamestate->projectiles[1]->entity->coords->x + dx, g_gamestate->projectiles[1]->entity->coords->y + dy) && contagem < DISTANCIA_ATIRAR_SMOKE) {
		    g_gamestate->projectiles[1]->entity->coords->x += dx;
		    g_gamestate->projectiles[1]->entity->coords->y += dy;
            contagem++;
        }
        else
            deploySmoke();


}

void deploySmoke() {

    for(int y = RAIO_SMOKE ; y >= -RAIO_SMOKE ; y--){
        for(int x = -RAIO_SMOKE ; x <= RAIO_SMOKE; x++) {
            if(x*x + y*y <= RAIO_SMOKE*RAIO_SMOKE && map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] != 1 && map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] != 3) 
                map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] = 8;
        }   
    }
    map[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x] = 8;

    for(int i = 0 ; i < g_gamestate->mob_count ; i++){
        if(map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] == 8)
            g_gamestate->mobs[i]->hasAI = FALSE;
    }
}

void removeSmoke() {

    for(int y = RAIO_SMOKE ; y >= -RAIO_SMOKE ; y--){
        for(int x = -RAIO_SMOKE ; x <= RAIO_SMOKE; x++){
            if(x*x + y*y <= RAIO_SMOKE*RAIO_SMOKE) 
                map[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x] = map_footprint[g_gamestate->projectiles[1]->entity->coords->y + y][g_gamestate->projectiles[1]->entity->coords->x + x];
        }
    }
    map[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x] = map_footprint[g_gamestate->projectiles[1]->entity->coords->y][g_gamestate->projectiles[1]->entity->coords->x];

    for(int i = 0 ; i < g_gamestate->mob_count ; i++) {
        map[g_gamestate->mobs[i]->entity->coords->y][g_gamestate->mobs[i]->entity->coords->x] = 5;
        g_gamestate->mobs[i]->hasAI = TRUE;
    }

    contagem = 0;
    g_gamestate->projectiles[1]->entity->coords->y = g_gamestate->projectiles[1]->entity->coords->x = 0;

}

void smokeChecker() {


    if( // se a volta do player nao for smoke, damos backup do raio de visao. isto evita o problema de ele sair da smoke e o backup ficar a 0.
        map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x+1] != 8 && 
        map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x-1] != 8 &&
        map[g_gamestate->player->entity->coords->y+1][g_gamestate->player->entity->coords->x] != 8 &&
        map[g_gamestate->player->entity->coords->y-1][g_gamestate->player->entity->coords->x] != 8
    )
        raioAnterior = g_gamestate->player->radius;

    g_gamestate->player->radius = (map[g_gamestate->player->entity->coords->y][g_gamestate->player->entity->coords->x] == 8) ? 0 : raioAnterior;

}