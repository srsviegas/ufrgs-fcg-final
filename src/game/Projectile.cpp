//
// Created by walte on 06/08/2024.
//

#include "Projectile.h"

#include <ctime>

Projectile::Projectile() {
    this->position = glm::vec4(0.0,0.0,0.0,0.0);
    this->direction = glm::vec4(0.0,0.0,0.0,0.0);
    this->speed = 0;
    this->accel = 0;
    this->lifetime = 0;
    this->startTime = 0;
    this->active = false;
}

void Projectile::shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float lifetime, float startTime) {
    this->position = start_pos;
    this->direction = direction;
    this->speed = speed;
    this->accel = accel;
    this->lifetime = lifetime;
    this->startTime = startTime;
    this->active = true;
}

void Projectile::deactivate() {
    active = false;
}

void Projectile::setPosition(glm::vec4 new_pos) {
    position = new_pos;
}

float Projectile::getStartTime() {
    return startTime;
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

float Projectile::getLifeTime() {
    return  lifetime;
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



bool Projectile::isActive() {
    return active;
}








