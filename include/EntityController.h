//
// Created by walte on 15/08/2024.
//

#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#define ENEMY_CHANCE_PER_TILE 30
#define MAX_ENTITIES 30

#include <glad/glad.h>

#include <algorithm>

#include "GameEntity.h"
#include "Player.h"
#include "Level.h"

class EntityController
{

public:
    EntityController();
    EntityController(Level levelData);
    void addEntity(GameEntity new_entity);
    void killAll();
    void step(float timeDelta, Player player);
    GameEntity *getEntities();

private:
    GameEntity entities[MAX_ENTITIES];
    int max_entities;
    int entity_count;
};

#endif // ENTITYCONTROLLER_H
