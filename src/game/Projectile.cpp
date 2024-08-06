//
// Created by walte on 06/08/2024.
//

#include "Projectile.h"

Projectile::Projectile(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime) {
    this->position = start_pos;
    this->direction = direction;
    this->speed = speed;
    this->accel = accel;
    this->lifetime = lifetime;
}

float Projectile::getAccel() const {
    return accel;
}

glm::vec4 Projectile::getDirection() {
    return  direction;
}

glm::vec4 Projectile::getPosition() {
    return  position;
}

float Projectile::getSpeed() const {
    return speed;
}

void Projectile::setAccel(float new_accel) {
    accel = new_accel;
}

void Projectile::setSpeed(float new_speed) {
    speed = new_speed;
}

void Projectile::step(float timeDelta) {
    speed += accel * timeDelta;
    position += timeDelta * speed * glm::normalize(direction);
}








