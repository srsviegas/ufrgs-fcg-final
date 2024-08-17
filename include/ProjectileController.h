//
// Created by walte on 13/08/2024.
//

#ifndef PROJETILECONTROLLER_H
#define PROJETILECONTROLLER_H

#include "Projectile.h"
#include "Constants.h"

class ProjectileController
{

public:
    ProjectileController();
    void shoot(int shooter_type, int shooter_id,int projectile_type, float cooldown, glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float damage, float lifetime, float startTime, glm::vec3 bbox_dimensions);
    void step(float current_time, float timeDelta);
    int getSize() const;
    bool onCooldown(int shooter_id, float current_time);
    Projectile *getProjectiles();

private:
    Projectile projectiles[MAX_PROJECTILES]{};
    float shooter_cooldown[MAX_PROJECTILES]{};
    float shooter_lastshot[MAX_PROJECTILES]{};

    int last_shot;
    int max_projectiles;
};

#endif // PROJETILECONTROLLER_H
