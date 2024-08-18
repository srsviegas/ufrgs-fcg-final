//
// Created by walte on 15/08/2024.
//

#include "EntityController.h"
#include <algorithm>
#include <iostream>

EntityController::EntityController()
{
    max_entities = MAX_ENTITIES;
    last_added = 0;
}

EntityController::EntityController(Level levelData)
{
    max_entities = MAX_ENTITIES;
    last_added = 0;
    for (int z = 0; z < levelData.GetMapHeight(); z++)
    {
        for (int x = 0; x < levelData.GetMapWidth(); x++)
        {
            bool isAroundSpawn = false;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    isAroundSpawn += levelData.IsPlayerSpawn(x + i, z + j);
                }
            }
            if (chance(ENEMY_CHANCE_PER_TILE) && levelData.IsFloor(x, z) && !isAroundSpawn)
            {
                if (chance(50))
                {
                    addEntity(
                        last_added,
                        ENTITY_CRAWLER,
                        100.0f,
                        1.0f,
                        3.5f,
                        1.5f,
                        levelData.MapPositionToWorldPosition(x, z),
                        glm::vec3(0.4f, 0.4f, 0.4f));
                }
                else
                {
                    addEntity(
                        last_added,
                        ENTITY_FLYER,
                        100.0f,
                        1.0f,
                        3.0f,
                        2.0f,
                        levelData.MapPositionToWorldPosition(x, z),
                        glm::vec3(0.4f, 0.4f, 0.4f));
                }
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

void EntityController::addEntity(int global_id, int type, float health, float walkSpeed, float range, float min_dist, glm::vec4 position, glm::vec3 bbox_dimensions)
{
    entities[last_added % MAX_ENTITIES] = {
        last_added,
        type,
        health,
        health,
        walkSpeed,
        range,
        min_dist,
        position,
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        bbox_dimensions,
        AABB{
            glm::vec3(-bbox_dimensions.x / 2, -bbox_dimensions.y / 2, -bbox_dimensions.z / 2) +
                glm::vec3(position.x, position.y, position.z),
            glm::vec3(bbox_dimensions.x / 2, bbox_dimensions.y / 2, bbox_dimensions.z / 2) +
                glm::vec3(position.x, position.y, position.z)},
        STATE_IDLE,
        true};
    last_added++;
}

void EntityController::step(float current_time, float timeDelta, Player *player, ProjectileController *projectiles)
{

    for (auto &entity : entities)
    {
        if (entity.status)
            switch (entity.type)
            {
            case ENTITY_FLYER:
                behaviour_flyer(&entity, current_time, timeDelta, player, projectiles);
                break;
            case ENTITY_RUNNER:
                break;
            case ENTITY_CRAWLER:
                behaviour_crawler(&entity, current_time, timeDelta, player, projectiles);
                break;
            default:
                std::cout << "invalid entity type";
                break;
            }
    }
}

void EntityController::behaviour_flyer(GameEntity *entity, float current_time, float timeDelta, Player *player, ProjectileController *projectiles)
{
    // update state;
    if (glm::length(entity->position - player->getPosition()) < entity->range)
    {
        entity->state = STATE_CHASING;
    }
    else
    {
        entity->state = STATE_ROAMING;
    }

    // step
    switch (entity->state)
    {
    case STATE_IDLE:
        break;
    case STATE_ROAMING:
        break;
    case STATE_CHASING:
        entity->target = player->getPosition();
        entity->direction = (glm::normalize(player->getPosition() - entity->position));
        if (glm::length(entity->position - player->getPosition()) > entity->min_dist)
        {
            entity->position += entity->direction * entity->walkSpeed * timeDelta;
        }
        if (!projectiles->onCooldown(entity->global_id, current_time))
        {
            projectiles->shoot(
                entity->type,
                entity->global_id,
                PROJECTILE_WATER,
                1.5f,
                entity->position,
                entity->direction,
                2.0f,
                0.0f,
                10.0f,
                2.5f,
                current_time,
                glm::vec3(0.3f, 0.3f, 0.3f));
        }
        break;
    default:
        std::cout << "invalid entity state";
        break;
    }
    entity->position.y = 0.35f;

    // update bounding box
    entity->bbox.min = glm::vec3(-entity->bbox_dimensions.x / 2, -entity->bbox_dimensions.y / 2, -entity->bbox_dimensions.z / 2) +
                       glm::vec3(entity->position.x, entity->position.y, entity->position.z),
    entity->bbox.max = glm::vec3(entity->bbox_dimensions.x / 2, entity->bbox_dimensions.y / 2, entity->bbox_dimensions.z / 2) +
                       glm::vec3(entity->position.x, entity->position.y, entity->position.z);
}

void EntityController::behaviour_crawler(GameEntity *entity, float current_time, float timeDelta, Player *player, ProjectileController *projectiles)
{
    // update state;
    if (glm::length(entity->position - player->getPosition()) < entity->range)
    {
        entity->state = STATE_CHASING;
    }
    else
    {
        entity->state = STATE_ROAMING;
    }

    // step
    switch (entity->state)
    {
    case STATE_IDLE:
        break;
    case STATE_ROAMING:
        break;
    case STATE_CHASING:
        entity->target = player->getPosition();
        entity->direction = (glm::normalize(player->getPosition() - entity->position));
        if (glm::length(entity->position - player->getPosition()) > entity->min_dist)
        {
            entity->position += entity->direction * entity->walkSpeed * timeDelta;
        }
        else
        {
            if (!projectiles->onCooldown(entity->global_id, current_time))
            {
                projectiles->shoot(
                    entity->type,
                    entity->global_id,
                    PROJECTILE_INVIS,
                    1.5f,
                    entity->position,
                    entity->direction,
                    2.0f,
                    0.0f,
                    10.0f,
                    0.5f,
                    current_time,
                    glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }
        break;
    default:
        std::cout << "invalid entity state";
        break;
    }
    entity->position.y = -0.85f;

    // update bounding box
    entity->bbox.min = glm::vec3(-entity->bbox_dimensions.x / 2, -entity->bbox_dimensions.y / 2, -entity->bbox_dimensions.z / 2) +
                       glm::vec3(entity->position.x, entity->position.y, entity->position.z),
    entity->bbox.max = glm::vec3(entity->bbox_dimensions.x / 2, entity->bbox_dimensions.y / 2, entity->bbox_dimensions.z / 2) +
                       glm::vec3(entity->position.x, entity->position.y, entity->position.z);
}

GameEntity *EntityController::getEntities()
{
    return entities;
}
