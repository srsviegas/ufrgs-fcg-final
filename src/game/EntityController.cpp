//
// Created by walte on 15/08/2024.
//

#include "EntityController.h"

#include <algorithm>
#include <Player.h>

EntityController::EntityController()
{
    max_entities = MAX_ENTITIES;
    last_added = 0;
}

EntityController::EntityController(Level levelData)
{
    for (int z = 0; z < levelData.GetMapHeight(); z++)
    {
        for (int x = 0; x < levelData.GetMapWidth(); x++) {
            if (chance(ENEMY_CHANCE_PER_TILE) && levelData.IsFloor(x, z) && !(levelData.IsPlayerSpawn(x, z))) {
                addEntity(
                    100.0f,
                    ENEMY_TYPE_1,
                    1.0f,
                    3.0f,
                    1.0f,
                    levelData.MapPositionToWorldPosition(x,z),
                    glm::vec3(0.4f, 0.4f, 0.4f));
            }
        }
    }
}

void EntityController::killAll()
{
    for (GameEntity &entity : entities)
    {
        entity.status = false;
    }
}

void EntityController::addEntity(float health, int type, float walkSpeed, float range, float min_dist, glm::vec4 position, glm::vec3 bbox_dimensions)
{
    entities[last_added % MAX_ENTITIES] = {
        health,
        health,
        ENEMY_TYPE_1,
        walkSpeed,
        range,
        min_dist,
        position,
        glm::vec4(1.0f,0.0f,0.0f,0.0f),
        glm::vec4(1.0f,0.0f,0.0f,0.0f),
        bbox_dimensions,
        AABB
        {
            glm::vec3(-bbox_dimensions.x/2,-bbox_dimensions.y/2,-bbox_dimensions.z/2) +
                glm::vec3(position.x,position.y,position.z),
            glm::vec3(bbox_dimensions.x/2,bbox_dimensions.y/2,bbox_dimensions.z/2) +
                glm::vec3(position.x,position.y,position.z)
        },
        STATE_IDLE,
        true
    };
    last_added++;
}

void EntityController::step(float timeDelta, Player player)
{
    for (auto &entity : entities)
    {
        if (entity.status){
            if (glm::length(entity.position - player.getPosition()) < entity.range){
                entity.state = STATE_CHASING;
            }else{
                entity.state = STATE_ROAMING;
            }
        }


        switch (entity.state)
        {
        case STATE_IDLE:
            break;
        case STATE_ROAMING:
            break;
        case STATE_CHASING:
            entity.target = player.getPosition();
            entity.direction = (glm::normalize(player.getPosition() - entity.position));
            if (glm::length(entity.position - player.getPosition()) > entity.min_dist)
            {
                entity.position += entity.direction * entity.walkSpeed * timeDelta;
            }
            break;
        default:
            break;
        }

        entity.bbox = AABB
        {
            glm::vec3(-entity.bbox_dimensions.x/2,-entity.bbox_dimensions.y/2,-entity.bbox_dimensions.z/2) +
                glm::vec3(entity.position.x,entity.position.y,entity.position.z),
            glm::vec3(entity.bbox_dimensions.x/2,entity.bbox_dimensions.y/2,entity.bbox_dimensions.z/2) +
                glm::vec3(entity.position.x,entity.position.y,entity.position.z)
        };
    }
}

GameEntity *EntityController::getEntities()
{
    return entities;
}
