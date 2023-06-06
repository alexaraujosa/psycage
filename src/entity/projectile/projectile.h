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
void init_grenades_clock();
void move_trap(int dx, int dy);
void deploy_trap();
void trap_checker();
void remove_trap();
void move_molotov(int dx, int dy);
void molotov_checker();
void remove_molotov();

#endif