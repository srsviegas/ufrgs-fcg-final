//
// Created by walte on 09/08/2024.
//

#include "GameEntity.h"

#include <glm/ext/quaternion_geometric.hpp>


GameEntity::GameEntity(glm::vec4 start_pos, int type, float health, float speed, float range) {
    this->position = start_pos;
    this->entity_type = type;
    this->health = health;
    this->walkSpeed = speed;
    this->range = range;
    this->state = STATE_IDLE;
    this->target = glm::vec4(0.0,0.0,0.0,0.0);
    this->direction = glm::vec4(1.0,0.0,0.0,0.0);
}

void GameEntity::set_health(float health) {
    this->health = health;
}

void GameEntity::set_walkspeed(float walkSpeed) {
    this->walkSpeed = walkSpeed;
}

void GameEntity::set_range(float range) {
    this->range = range;
}

void GameEntity::set_position(const glm::vec4 &position) {
    this->position = position;
}

void GameEntity::set_direction(const glm::vec4 &direction) {
    this->direction = direction;
}

void GameEntity::set_target(const glm::vec4 &target) {
    this->target = target;
}

void GameEntity::set_state(int state) {
    this->state = state;
}

float GameEntity::get_health() const {
    return health;
}

float GameEntity::get_walkspeed() const {
    return walkSpeed;
}

float GameEntity::get_range() const {
    return range;
}

glm::vec4 GameEntity::get_position() const {
    return position;
}

glm::vec4 GameEntity::get_direction() const {
    return direction;
}

glm::vec4 GameEntity::get_target() const {
    return target;
}

int GameEntity::get_state() const {
    return state;
}

void GameEntity::step(float timeDelta) {
    switch (state) {
        case STATE_IDLE:
            break;
        case STATE_ROAMING:
            break;
        case STATE_CHASING:
            direction = target - position;
            position += glm::normalize(direction) * walkSpeed;
            break;
        default:
            break;
    }
}