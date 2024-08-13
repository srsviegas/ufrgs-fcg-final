#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <glm/glm.hpp>

#include <algorithm> // min and max functions
#include <stdio.h>   // Debugging

#include <Level.h>

struct AABB
{
    glm::vec3 min;
    glm::vec3 max;

    void DebugPrint()
    {
        printf("MIN:(%f, %f, %f)\t MAX:(%f, %f, %f)\n",
               min.x, min.y, min.z,
               max.x, max.y, max.z);
    }
};

class Sphere
{
public:
    Sphere(glm::vec3 c, float r);
    Sphere(glm::vec4 c, float r);
    bool IsCollidingAABB(AABB aabb);
    bool IsCollidingPlane(Plane plane);

private:
    glm::vec3 center;
    float radius;
    float radiusSquared;
};

#endif