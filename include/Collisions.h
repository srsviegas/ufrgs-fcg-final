#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>

#include <algorithm> // min and max functions

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;
};

class Sphere
{
public:
    Sphere(glm::vec3 c, float r);
    Sphere(glm::vec4 c, float r);
    bool IsCollidingAABB(AABB aabb);

private:
    glm::vec3 center;
    float radius;
};

#endif