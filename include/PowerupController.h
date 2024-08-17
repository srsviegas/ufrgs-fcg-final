//
// Created by walte on 16/08/2024.
//

#ifndef POWERUPCONTROLLER_H
#define POWERUPCONTROLLER_H
#include <Player.h>
#include <glm/vec4.hpp>
#include "BezierCurve.h"

#define MAX_POWERUPS 10
#define POWERUP_COLLECT_SPEED 0.7f

struct Powerup
{
    float health;
    float mana;
    int model_type;

    bool isActive;
    BezierCurve trajectory;
    float step;
};


class PowerupController {
public:
    PowerupController();
    void spawn(glm::vec4 start_pos, float health, float mana, int model_type);
    void step(Player *player, float timeDelta);
    Powerup* getPowerUps();

private:
    int last_spawned;
    Powerup powerups[MAX_POWERUPS] = {};

};

#endif //POWERUPCONTROLLER_H
