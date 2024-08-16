//
// Created by walte on 09/08/2024.
//

#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define STATE_IDLE 0
#define STATE_ROAMING 1
#define STATE_CHASING 2

class GameEntity
{
public:
    GameEntity(glm::vec4 start_pos, int type, float health, float speed, float range);
    GameEntity();

    void set_health(float health);
    void set_walkspeed(float walkSpeed);
    void set_range(float range);
    void set_position(const glm::vec4 &position);
    void set_direction(const glm::vec4 &direction);
    void set_target(const glm::vec4 &target);
    void set_state(int state);

    float get_health() const;
    float get_walkspeed() const;
    float get_range() const;
    glm::vec4 get_position() const;
    glm::vec4 get_direction() const;
    glm::vec4 get_target() const;
    int get_state() const;
    bool is_status() const;
    void set_status(bool status);

private:
    float health;
    float walkSpeed;
    float range; //distãncia de detecção máxima
    float min_dist; //distância mínima que a entidade se aproximará do jogador
    int entity_type;

    glm::vec4 position;
    glm::vec4 direction;
    glm::vec4 target;
    int state;
    bool status;
};

#endif // GAMEENTITY_H
