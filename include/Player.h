//
// Created by walte on 04/08/2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#define MAX_HEALTH 100
#define MAX_MANA 100
#define MANA_REGENFACTOR 5
#define HEALTH_REGENFACTOR 0.1
#define DEFAULT_SPEED 1.0

#include <glm/vec4.hpp>

class Player
{
public:
    explicit Player(glm::vec4 start_pos);
    void setWalkspeed(float speed);
    void setPosition(glm::vec4 new_pos);
    void move(glm::vec4 displacement);
    void setHealth(float hp);
    void setMana(float amount);
    void damage(float hp);

    float getWalkspeed() const;
    glm::vec4 getPosition() const;
    float getHealth() const;
    float getHealthPercent() const;
    float getMana() const;
    float getManaPercent() const;

private:
    float health;
    float mana;
    float walkspeed;
    glm::vec4 player_pos{};
};



#endif //PLAYER_H
