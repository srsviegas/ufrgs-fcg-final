//
// Created by walte on 04/08/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <glm/vec4.hpp>
#include "Level.h"
#include "Collisions.h"
#include "Constants.h"

class Player
{
public:
    Player();
    explicit Player(glm::vec4 start_pos);
    void setWalkSpeed(float speed);
    void setPosition(glm::vec4 new_pos);
    void move(float timeDelta, glm::vec4 movementDirection, Level levelData);
    void setHealth(float hp);
    void setMana(float amount);
    void damage(float hp);
    bool IsDead();

    float getWalkSpeed() const;
    glm::vec4 getPosition() const;
    float getHealth() const;
    float getHealthPercent() const;
    float getMana() const;
    float getManaPercent() const;

    Sphere GetCollision();

    void update(float timeDelta);

private:
    float health;
    float mana;
    float walkSpeed;
    float collisionRadius;

    glm::vec4 player_pos{};
};

#endif // PLAYER_H
