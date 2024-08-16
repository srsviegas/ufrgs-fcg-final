//
// Created by walte on 15/08/2024.
//

#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include <glad/glad.h>
#define MAX_ENTITIES 10

#include <Player.h>

#include "GameEntity.h"

class EntityController {

public:
    EntityController();
    void addEntity(GameEntity new_entity);
    void killAll();
    void step(float timeDelta, Player player);
    GameEntity* getEntities();

private:
    GameEntity entities[MAX_ENTITIES];
    int max_entities;
    int entity_count;
};



#endif //ENTITYCONTROLLER_H
