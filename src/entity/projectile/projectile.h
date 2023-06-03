/***************************************************************
 * PROJECTILE MODULE                                           *
 *                                                             *
 * This module handles the projectile behavior.                *
 ***************************************************************/

#ifndef __RL_PROJECTILE_H
#define __RL_PROJECTILE_H

#include "../entity.h"

typedef struct projectile {
    Entity entity;
    int dx;
    int dy;
} PROJECTILE, *Projectile;

Projectile defaultProjectile();
void destroyProjectile(Projectile projectile);
void moveSmoke(int dx, int dy);
void deploySmoke();
void smokeChecker();
void removeSmoke();

#endif