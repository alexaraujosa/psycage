#include "projectile.h"
#include <string.h>

Projectile defaultProjectile() {
    Projectile projectile = (Projectile)malloc(sizeof(PROJECTILE));
    if (projectile == NULL) return NULL;

    Entity entity = defaultEntity();
    if (entity == NULL) return NULL;

    projectile->entity = entity;

    return projectile;
}

void destroyProjectile(Projectile projectile) {
    free(projectile->entity);
    free(projectile);
}