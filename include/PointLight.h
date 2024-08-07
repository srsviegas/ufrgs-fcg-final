//
// Created by walte on 07/08/2024.
//

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

class PointLight {
public:
    PointLight(glm::vec4 position, glm::vec3 color_rgb, float intensity);
    void setPosition(glm::vec4 position);
    void setIntensity(float intens);
    void setColor(glm::vec3 color);
    void enable();
    void disable();

    glm::vec4 getPosition();
    glm::vec3 getColor();
    float getIntensity();
    bool isActive();

private:
    glm::vec4 position;
    glm::vec3 color;
    float intensity;
    bool status;
};

#endif //POINTLIGHT_H
