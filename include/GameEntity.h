//
// Created by walte on 09/08/2024.
//

#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Collisions.h"

struct GameEntity {
    float health;
    float maxhealth;
    int type;
    float walkSpeed;
    float range;    // distãncia de detecção máxima
    float min_dist; // distância mínima que a entidade se aproximará do jogador

    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 target;
    glm::vec3 bbox_dimensions;
    AABB bbox;

    int state;
    bool status;

};

#endif // GAMEENTITY_H
