//
// Created by walte on 05/08/2024.
//
#include "Player.h"
#include <stdio.h>

Player::Player() : Player(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}

Player::Player(glm::vec4 start_pos)
{
    health = MAX_HEALTH;
    mana = MAX_MANA;
    walkSpeed = DEFAULT_SPEED;
    player_pos = start_pos;
    collisionRadius = DEFAULT_COLLISIONRADIUS;
}

void Player::setWalkSpeed(float speed)
{
    walkSpeed = speed;
}

void Player::setPosition(glm::vec4 new_pos)
{
    player_pos = new_pos;
}

void Player::move(float timeDelta, glm::vec4 movementDirection, Level levelData)
{
    int mapPositionX = levelData.WorldPositionToMapPositionX(getPosition().x);
    int mapPositionZ = levelData.WorldPositionToMapPositionZ(getPosition().z);

    glm::vec4 intendedMovement = timeDelta * getWalkSpeed() * movementDirection;
    setPosition(getPosition() + intendedMovement);

    Sphere playerSphere = GetCollision();

    for (auto &plane : levelData.GetWallsAtTile(mapPositionX, mapPositionZ))
    {
        if (playerSphere.IsCollidingPlane(plane))
        {
            float normalProjection = dotproduct(intendedMovement, plane.normal);

            if (normalProjection >= 0)
            {
                // Move player from the back side of the wall
                setPosition(getPosition() + plane.normal * glm::vec4(0.05, 0.05, 0.05, 0.0));
                continue;
            }

            glm::vec4 projectedMovement = intendedMovement - normalProjection * plane.normal;
            setPosition(getPosition() + projectedMovement - intendedMovement);
        }
    }
}

void Player::setHealth(float hp)
{
    health = hp;
}

void Player::setMana(float amount)
{
    mana = amount;
}

void Player::damage(float hp)
{
    health -= hp;
}

float Player::getWalkSpeed() const
{
    return walkSpeed;
}

float Player::getHealth() const
{
    return health;
}

float Player::getMana() const
{
    return mana;
}

glm::vec4 Player::getPosition() const
{
    return player_pos;
}

float Player::getHealthPercent() const
{
    return health / MAX_HEALTH;
}

float Player::getManaPercent() const
{
    return mana / MAX_MANA;
}

Sphere Player::GetCollision()
{
    return Sphere(player_pos, DEFAULT_COLLISIONRADIUS);
}

void Player::update(float timeDelta)
{
    if (mana < MAX_MANA)
    {
        mana += timeDelta * MANA_REGENFACTOR;
    }
    if (health < MAX_HEALTH)
    {
        health += timeDelta * HEALTH_REGENFACTOR;
    }
}