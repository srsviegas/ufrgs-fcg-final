//
// Created by walte on 15/08/2024.
//

#include "EntityController.h"

#include <algorithm>
#include <Player.h>

EntityController::EntityController() {
    for(GameEntity & entity : entities) {
        entity = GameEntity();
    }
}

void EntityController::killAll() {
    for(GameEntity & entity : entities) {
        entity.setStatus(false);
    }
}

void EntityController::addEntity(GameEntity new_entity) {
    entities[entity_count % MAX_ENTITIES] = new_entity;
}

void EntityController::step(float timeDelta, Player player) {
    for(auto & entity : entities){
        if(entity.isActive()) {
            if(glm::length(entity.getPosition() - player.getPosition()) < entity.getRange()) {
                entity.setState(STATE_CHASING);
            } else {
                entity.setState((STATE_IDLE));
            }
        }
            switch (entity.getState()) {
                case STATE_IDLE:
                    break;
                case STATE_ROAMING:
                    break;
                case STATE_CHASING:
                    entity.setTarget(player.getPosition());
                    entity.setDirection(glm::normalize( player.getPosition() - entity.getPosition()));
                    if(glm::length(entity.getPosition() - player.getPosition()) > entity.getMinDist()) {
                        entity._move(entity.getDirection() * entity.getWalkspeed() * timeDelta);
                    }
                    break;
                default:
                    break;
            }
    }
}

GameEntity* EntityController::getEntities() {
    return  entities;
}



