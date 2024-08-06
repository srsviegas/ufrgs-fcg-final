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
        Projectile(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime);
        void setSpeed(float new_speed);
        void setAccel(float new_accel);
        void setPosition(glm::vec4 new_pos);
        void setDirection(glm::vec4 new_dir);

        float getSpeed() const;
        float getAccel() const;
        glm::vec4 getPosition();
        glm::vec4 getDirection();
        void step(float timeDelta);

private:
        glm::vec4 position;
        glm::vec4 direction;
        float speed;
        float accel;
        float lifetime;

};

#endif //PROJECTILE_H
