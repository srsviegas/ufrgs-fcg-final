//
// Created by walte on 13/08/2024.
//

#ifndef PROJETILECONTROLLER_H
#define PROJETILECONTROLLER_H

#include "Projectile.h"

#define MAX_PROJECTILES 10

class ProjectileController {

public:
    ProjectileController(int size, float cooldown);
    void shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime, float startTime);
    void step(float current_time, float timeDelta);
    Projectile* getProjectiles();
    int getSize() const;
    bool onCooldown(float current_time) const;


private:
    Projectile projectiles[MAX_PROJECTILES];
    float cooldown;
    float last_shot_time;
    int last_shot;
    int max_projectiles;
};


#endif //PROJETILECONTROLLER_H
