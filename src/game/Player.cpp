//
// Created by walte on 05/08/2024.
//
#include "Player.h"
#include <stdio.h>

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
    int oldMapPositionX = levelData.WorldPositionToMapPositionX(player_pos.x);
    int oldMapPositionZ = levelData.WorldPositionToMapPositionZ(player_pos.z);

    /* TESTING */
    for (auto &plane : levelData.GetPlanesAtTile(oldMapPositionX, oldMapPositionZ))
    {
        plane.DebugPrint();
    }
    printf("\n");
    /* TESTING */

    glm::vec4 newWorldPosition = player_pos + (timeDelta * getWalkSpeed() * movementDirection);
    int newMapPositionX = levelData.WorldPositionToMapPositionX(newWorldPosition.x + collisionRadius * (movementDirection.x > 0 ? 1 : -1));
    int newMapPositionZ = levelData.WorldPositionToMapPositionZ(newWorldPosition.z + collisionRadius * (movementDirection.z > 0 ? 1 : -1));

    if (levelData.IsFloor(newMapPositionX, newMapPositionZ))
    {
        player_pos = newWorldPosition;
    }
    else if (levelData.IsFloor(newMapPositionX, oldMapPositionZ))
    {
        player_pos.x = newWorldPosition.x;
    }
    else if (levelData.IsFloor(oldMapPositionX, newMapPositionZ))
    {
        player_pos.z = newWorldPosition.z;
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