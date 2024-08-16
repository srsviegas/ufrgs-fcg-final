//
// Created by walte on 05/08/2024.
//

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Collisions.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"

class Projectile {
public:
        Projectile();
        void shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float damage, float lifetime, float startTime, glm::vec3 bbox_dimensions);
        void setSpeed(float new_speed);
        void setAccel(float new_accel);
        void setPosition(glm::vec4 new_pos);
        void setDirection(glm::vec4 new_dir);

        bool isActive();
        float getStartTime();
        float getLifeTime();
        float getSpeed() const;
        float getAccel() const;
        float getDamage() const;
        glm::vec4 getPosition();
        glm::vec4 getDirection();
        void deactivate();
        AABB getBoundingBox();

private:
        bool active;
        float startTime;
        glm::vec4 position;
        glm::vec4 direction;
        float speed;
        float accel;
        float damage;
        float lifetime;
        glm::vec3 bbox_dimensions;

};

#endif //PROJECTILE_H
