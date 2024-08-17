//
// Created by walte on 05/08/2024.
//

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Collisions.h"
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"

struct Projectile
{
        int shooter_type;
        int shooter_id;

        float startTime;
        float speed;
        float accel;
        float damage;
        float lifetime;
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec3 bbox_dimensions;
        AABB bbox;
        bool status;
};

#endif //PROJECTILE_H
