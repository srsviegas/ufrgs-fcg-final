//
// Created by walte on 15/08/2024.
//

#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include <GameEntity.h>
#include "Player.h"
#include "Level.h"
#include "Constants.h"

class EntityController
{

public:
    EntityController();
    EntityController(Level levelData);
    void addEntity(float health, int type, float walkSpeed, float range, float min_dist, glm::vec4 position, glm::vec3 bbox_dimensions);
    void killAll();
    void step(float timeDelta, Player player);
    GameEntity *getEntities();

private:
    GameEntity entities[MAX_ENTITIES]{};
    int max_entities;
    int last_added;
};

#endif // ENTITYCONTROLLER_H
