//
// Created by walte on 15/08/2024.
//

#include "EntityController.h"

#include <algorithm>

EntityController::EntityController() {
    for(GameEntity & entity : entities) {
        entity = GameEntity();
    }
}

void EntityController::killAll() {
    for(GameEntity & entity : entities) {
        entity.set_status(false);
    }
}

void EntityController::addEntity(GameEntity new_entity) {
    entities[entity_count % MAX_ENTITIES] = new_entity;
}




