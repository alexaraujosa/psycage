#include "projectile.h"
#include <string.h>

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
    free(projectile->entity);
    free(projectile);
}