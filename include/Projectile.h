//
// Created by walte on 05/08/2024.
//

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"

class Projectile {
public:
        Projectile();
        void shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime, float startTime);
        void setSpeed(float new_speed);
        void setAccel(float new_accel);
        void setPosition(glm::vec4 new_pos);
        void setDirection(glm::vec4 new_dir);

        bool isActive();
        float getStartTime();
        float getLifeTime();
        float getSpeed() const;
        float getAccel() const;
        glm::vec4 getPosition();
        glm::vec4 getDirection();
        void deactivate();
        void step(float timeDelta);

private:
        bool active;
        float startTime;
        glm::vec4 position;
        glm::vec4 direction;
        float speed;
        float accel;
        float lifetime;

};

#endif //PROJECTILE_H
