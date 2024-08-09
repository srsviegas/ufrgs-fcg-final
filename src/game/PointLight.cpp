//
// Created by walte on 07/08/2024.
//

#include "PointLight.h"

PointLight::PointLight(glm::vec4 position, glm::vec3 color_rgb, float intensity) {
    this->position = position;
    this->color = color_rgb;
    this->intensity = intensity;
}

PointLight::PointLight() {
    this->position = glm::vec4(0.0f,0.0f,0.0f,0.0f);
    this->color = glm::vec3(0.0f,0.0f,0.0f);
    this->intensity = 0;
}

void PointLight::disable() {
    this->intensity = 0.0f;
}

void PointLight::enable() {
    this->intensity = 1.0f;
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
    return intensity > 0;
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









