//
// Created by walte on 13/08/2024.
//

#include "ProjectileController.h"

ProjectileController::ProjectileController() {
    last_shot = 0;
    max_projectiles = MAX_PROJECTILES;
}

void ProjectileController::shoot(
    int shooter_type,
    int shooter_id,
    int projectile_type,
    float cooldown,
    glm::vec4 start_pos,
    glm::vec4 direction,
    float speed,
    float accel,
    float damage,
    float lifetime,
    float startTime,
    glm::vec3 bbox_dimensions)
    {
        projectiles[last_shot % MAX_PROJECTILES] =
        {
            shooter_type,
            shooter_id,
            projectile_type,
            startTime,
            speed,
            accel,
            damage,
            lifetime,
            start_pos,
            direction,
            bbox_dimensions,
            AABB
                {
                glm::vec3(-bbox_dimensions.x/2,-bbox_dimensions.y/2,-bbox_dimensions.z/2) +
                    glm::vec3(start_pos.x,start_pos.y,start_pos.z),
               glm::vec3(bbox_dimensions.x/2,bbox_dimensions.y/2,bbox_dimensions.z/2) +
                   glm::vec3(start_pos.x,start_pos.y,start_pos.z)
                },
            true,
        };
        shooter_cooldown[shooter_id] = cooldown;
        shooter_lastshot[shooter_id] = startTime;
        last_shot++;
}

bool ProjectileController::onCooldown(int shooter_id, float current_time) {
    return current_time - shooter_lastshot[shooter_id] < shooter_cooldown[shooter_id];
}

void ProjectileController::step(float current_time, float timeDelta) {
    for (auto &projectile : projectiles)
    {
        if (projectile.status)
        {
            projectile.speed += projectile.accel * timeDelta;
            projectile.position += projectile.speed *glm::normalize(projectile.direction) * timeDelta;

            if (current_time - projectile.startTime > projectile.lifetime)
            {
                projectile.status = false;
            }
            projectile.bbox.min = glm::vec3(-projectile.bbox_dimensions.x/2,-projectile.bbox_dimensions.y/2,-projectile.bbox_dimensions.z/2) +
                        glm::vec3(projectile.position.x,projectile.position.y,projectile.position.z);
            projectile.bbox.max = glm::vec3(projectile.bbox_dimensions.x/2,projectile.bbox_dimensions.y/2,projectile.bbox_dimensions.z/2) +
                       glm::vec3(projectile.position.x,projectile.position.y,projectile.position.z);
        }
    }
}

Projectile* ProjectileController::getProjectiles() {
    return projectiles;
}

int ProjectileController::getSize() const {
    return max_projectiles;
}



