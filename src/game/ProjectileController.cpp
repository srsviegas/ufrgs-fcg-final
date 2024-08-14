//
// Created by walte on 13/08/2024.
//

#include "ProjectileController.h"

ProjectileController::ProjectileController(int size, float cooldown) {
    for (Projectile & projectile : projectiles)
    {
        projectile = Projectile();
    }
    this->cooldown = cooldown;
    last_shot = 0;
    last_shot_time = 0.0f;
    max_projectiles = size;
}

void ProjectileController::shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime, float startTime) {
        projectiles[last_shot % MAX_PROJECTILES].shoot(start_pos,direction,speed,accel,lifetime,startTime);
        last_shot++;
        last_shot_time = startTime;
}

void ProjectileController::step(float current_time, float timeDelta) {
    for (auto &projectile : projectiles)
    {
        if (projectile.isActive())
        {
            projectile.step(timeDelta);
            if (current_time - projectile.getStartTime() > projectile.getLifeTime())
            {
                projectile.deactivate();
            }
        }
    }
}

Projectile* ProjectileController::getProjectiles() {
    return projectiles;
}

int ProjectileController::getSize() const {
    return max_projectiles;
}

bool ProjectileController::onCooldown(float current_time) const {
    return current_time - last_shot_time < cooldown;
}


