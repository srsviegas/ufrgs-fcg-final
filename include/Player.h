//
// Created by walte on 04/08/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_HEALTH 100
#define MAX_MANA 1000
#define MANA_REGENFACTOR 5
#define HEALTH_REGENFACTOR 0.1
#define DEFAULT_SPEED 2.5
#define DEFAULT_COLLISIONRADIUS 0.5

#include <glm/vec4.hpp>
#include "Level.h"

class Player
{
public:
    explicit Player(glm::vec4 start_pos);
    void setWalkSpeed(float speed);
    void setPosition(glm::vec4 new_pos);
    void move(float timeDelta, glm::vec4 movementDirection, Level levelData);
    void setHealth(float hp);
    void setMana(float amount);
    void damage(float hp);

    float getWalkSpeed() const;
    glm::vec4 getPosition() const;
    float getHealth() const;
    float getHealthPercent() const;
    float getMana() const;
    float getManaPercent() const;

    void update(float timeDelta);

private:
    float health;
    float mana;
    float walkSpeed;
    float collisionRadius;
    glm::vec4 player_pos{};
};

#endif // PLAYER_H
