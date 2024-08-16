//
// Created by walte on 09/08/2024.
//

#include "GameEntity.h"

#include <glm/ext/quaternion_geometric.hpp>


GameEntity::GameEntity(glm::vec4 start_pos, int type, float health, float speed, float range, float min_dist, glm::vec3 bbox_dimensions) {
    this->position = start_pos;
    this->entity_type = type;
    this->health = health;
    this->walkSpeed = speed;
    this->range = range;
    this->min_dist = min_dist;

    this->state = STATE_IDLE;
    this->target = glm::vec4(0.0,0.0,0.0,0.0);
    this->direction = glm::vec4(1.0,0.0,0.0,0.0);
    this->status = true;
    this->bbox_dimension = bbox_dimensions;
}

GameEntity::GameEntity() {
    this->position = glm::vec4(0.0,0.0,0.0,0.0);;
    this->entity_type = 0;
    this->health = 0;
    this->walkSpeed = 0;
    this->range = 0;
    this->status = false;
    this->state = STATE_IDLE;
    this->min_dist = 0;
    this->target = glm::vec4(0.0,0.0,0.0,0.0);
    this->direction = glm::vec4(1.0,0.0,0.0,0.0);
}


void GameEntity::setHealth(float health) {
    this->health = health;
}

void GameEntity::setWalkspeed(float walkSpeed) {
    this->walkSpeed = walkSpeed;
}

void GameEntity::setRange(float range) {
    this->range = range;
}

void GameEntity::setPosition(const glm::vec4 &position) {
    this->position = position;
}

void GameEntity::setDirection(const glm::vec4 &direction) {
    this->direction = direction;
}

void GameEntity::setTarget(const glm::vec4 &target) {
    this->target = target;
}

void GameEntity::setState(int state) {
    this->state = state;
}

float GameEntity::getHealth() const {
    return health;
}

float GameEntity::getWalkspeed() const {
    return walkSpeed;
}

float GameEntity::getRange() const {
    return range;
}

glm::vec4 GameEntity::getPosition() const {
    return position;
}

glm::vec4 GameEntity::getDirection() const {
    return direction;
}

glm::vec4 GameEntity::getTarget() const {
    return target;
}

int GameEntity::getState() const {
    return state;
}

void GameEntity::setStatus(bool status) {
    this->status = status;
}

bool GameEntity::isActive() const {
    return  status;
}

void GameEntity::_move(glm::vec4 displacement) {
    position += displacement;
}

float GameEntity::getMinDist() const {
    return min_dist;
}



AABB GameEntity::getBoundingBox() const {

    return AABB
    {
        glm::vec3(-bbox_dimension.x/2,-bbox_dimension.y/2,-bbox_dimension.z/2),
        glm::vec3(bbox_dimension.x/2,bbox_dimension.y/2,bbox_dimension.z/2)
    };
}



