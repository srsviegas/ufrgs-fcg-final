//
// Created by walte on 05/08/2024.
//
#include "Player.h"

Player::Player(glm::vec4 start_pos) {
    health = MAX_HEALTH;
    mana = MAX_MANA;
    walkspeed = DEFAULT_SPEED;
    player_pos = start_pos;
}

void Player::setWalkspeed(float speed) {
    walkspeed = speed;
}

void Player::setPosition(glm::vec4 new_pos) {
    player_pos = new_pos;
}

void Player::move(glm::vec4 displacement) {
    player_pos += displacement;
}

void Player::setHealth(float hp) {
    health = hp;
}

void Player::setMana(float amount) {
    mana = amount;
}

void Player::damage(float hp) {
    health -= hp;
}

float Player::getWalkspeed() const {
    return  walkspeed;
}

float Player::getHealth() const {
    return health;
}

float Player::getMana() const {
    return  mana;
}

glm::vec4 Player::getPosition() const {
    return  player_pos;
}

float Player::getHealthPercent() const {
    return health/MAX_HEALTH;
}








