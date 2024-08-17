//
// Created by walte on 15/08/2024.
//

#ifndef ENTITYCONTROLLER_H
#define ENTITYCONTROLLER_H

#include "GameEntity.h"
#include "Player.h"
#include "Level.h"
#include "Constants.h"
#include "ProjectileController.h"


class EntityController
{

public:
    EntityController();
    EntityController(Level levelData);
    void addEntity(int global_id, int type, float health, float walkSpeed, float range, float min_dist, glm::vec4 position, glm::vec3 bbox_dimensions);
    void killAll();
    void step(float current_time, float timeDelta, Player *player, ProjectileController *projectiles);
    GameEntity *getEntities();

private:
    GameEntity entities[MAX_ENTITIES]{};
    int max_entities;
    int last_added;

    void behaviour_flyer(GameEntity *entity, float current_time, float timeDelta, Player *player, ProjectileController *projectiles);
    void behaviour_runner(GameEntity *entity, float current_time, float timeDelta, Player *player, ProjectileController *projectiles);
    void behaviour_crawler(GameEntity *entity, float current_time, float timeDelta, Player *player, ProjectileController *projectiles);
};

#endif // ENTITYCONTROLLER_H
