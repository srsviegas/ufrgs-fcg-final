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
        int shooter_type; //tipo de entidade que disparou - jogador, entidade1, entidade2
        int shooter_id; //identificador único da entidade que disparou, evita compartilhar cooldown

        int projectile_type; //aparência do projétil
        float startTime; //timestamp do momento que foi disparado
        float speed; //velocidade
        float accel; //aceleração
        float damage; //dano ao colidir
        float lifetime; //tempo máximo de vida
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec3 bbox_dimensions; //tamanho da área de colisão
        AABB bbox; //(derivado)área de colisão
        bool status; //estado - ativo, inativo
};

#endif //PROJECTILE_H
