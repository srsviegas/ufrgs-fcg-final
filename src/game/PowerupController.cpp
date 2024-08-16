//
// Created by walte on 16/08/2024.
//

#include "PowerupController.h"

#include <Player.h>


PowerupController::PowerupController() {
    this->last_spawned = 0;
    this->powerups;
    for(auto & powerup : powerups) {
        powerup.health = 0.0f;
        powerup.mana = 0.0f;
        powerup.model_type = 0;
        powerup.step = 0.0f;
        powerup.trajectory = BezierCurve();
    }
}

void PowerupController::spawn(glm::vec4 start_pos,float health, float mana, int model_type) {
    powerups[last_spawned % MAX_POWERUPS].health = health;
    powerups[last_spawned % MAX_POWERUPS].mana = mana;
    powerups[last_spawned % MAX_POWERUPS].model_type = model_type;

    powerups[last_spawned % MAX_POWERUPS].isActive = true;
    powerups[last_spawned % MAX_POWERUPS].trajectory.setP1(start_pos);
    powerups[last_spawned % MAX_POWERUPS].trajectory.setP2(start_pos);
}

void PowerupController::step(Player player, float timeDelta) {
    for(auto & powerup : powerups) {
        if(powerup.isActive) {
            powerup.trajectory.setP3(player.getPosition() + glm::vec4(0.0f,1.0f,0.0f,0.0f));
            powerup.trajectory.setP4(player.getPosition() + glm::vec4(0.0f,-0.5f,0.0f,0.0f));
            powerup.step += POWERUP_COLLECT_SPEED * timeDelta;
            if(powerup.step > 1.0f) {
                player.setHealth(player.getHealth() + powerup.health);
                player.setMana(player.getMana() + powerup.mana);
                powerup.isActive = false;
            }
        }
    }
}

Powerup *PowerupController::getPowerUps() {
    return  powerups;
}



