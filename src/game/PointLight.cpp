//
// Created by walte on 07/08/2024.
//

#include "PointLight.h"

#include <_mingw_stat64.h>

PointLight::PointLight(glm::vec4 position, glm::vec3 color_rgb, float intensity) {
    this->position = position;
    this->color = color_rgb;
    this->intensity = intensity;
}

void PointLight::disable() {
    status = false;
}

void PointLight::enable() {
    status = true;
}

glm::vec3 PointLight::getColor() {
    return color;
}

float PointLight::getIntensity() {
    return  intensity;
}

glm::vec4 PointLight::getPosition() {
    return  position;
}

bool PointLight::isActive() {
    return status;
}

void PointLight::setColor(glm::vec3 color) {
    this->color = color;
}

void PointLight::setIntensity(float intens) {
    this->intensity = intens;
}

void PointLight::setPosition(glm::vec4 position) {
    this->position = position;
}









