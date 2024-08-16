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
    this->bbox_dimensions = glm::vec3(1.0f,1.0f,1.0f);
}

void Projectile::shoot(glm::vec4 start_pos, glm::vec4 direction, float speed, float accel, float damage, float lifetime, float startTime, glm::vec3 bbox_dimensions) {
    this->position = start_pos;
    this->direction = direction;
    this->speed = speed;
    this->accel = accel;
    this->damage = damage;
    this->lifetime = lifetime;
    this->startTime = startTime;
    this->active = true;
    this->bbox_dimensions = bbox_dimensions;
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

float Projectile::getDamage() const {
    return  damage;
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

AABB Projectile::getBoundingBox() {
    return AABB
    {
         glm::vec3(-bbox_dimensions.x/2,-bbox_dimensions.y/2,-bbox_dimensions.z/2) +
             glm::vec3(position.x,position.y,position.z),

        glm::vec3(bbox_dimensions.x/2,bbox_dimensions.y/2,bbox_dimensions.z/2) +
            glm::vec3(position.x,position.y,position.z)
    };
}




bool Projectile::isActive() {
    return active;
}








