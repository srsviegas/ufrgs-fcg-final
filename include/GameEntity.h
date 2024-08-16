//
// Created by walte on 09/08/2024.
//

#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Collisions.h"

#define STATE_IDLE 0
#define STATE_ROAMING 1
#define STATE_CHASING 2

class GameEntity
{
public:
    GameEntity(glm::vec4 start_pos, int type, float health, float speed, float range, float min_dist, glm::vec3 bbox_dimensions);
    GameEntity();

    void setHealth(float health);
    void damage(float amount);
    void setWalkspeed(float walkSpeed);
    void setRange(float range);
    void setPosition(const glm::vec4 &position);
    void setDirection(const glm::vec4 &direction);
    void setTarget(const glm::vec4 &target);
    void setState(int state);

    float getHealth() const;
    float getWalkspeed() const;
    float getRange() const;
    float getMinDist() const;
    glm::vec4 getPosition() const;
    glm::vec4 getDirection() const;
    glm::vec4 getTarget() const;
    int getState() const;
    bool isActive() const;
    void setStatus(bool status);
    void _move(glm::vec4 displacement);
    AABB getBoundingBox() const;

private:
    float health;
    float walkSpeed;
    float range; //distãncia de detecção máxima
    float min_dist; //distância mínima que a entidade se aproximará do jogador
    int entity_type;

    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 target;
    glm::vec3 bbox_dimension;
    int state;
    bool status;
};

#endif // GAMEENTITY_H
