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

inline bool isColliding(AABB bbox_1, AABB bbox_2) {
    return
    bbox_1.max.x > bbox_2.min.x &&
        bbox_1.min.x < bbox_2.max.x &&
            bbox_1.max.y > bbox_2.min.y &&
                bbox_1.min.y < bbox_2.max.y &&
                    bbox_1.max.z > bbox_2.min.z &&
                        bbox_1.min.z < bbox_2.max.z;

}

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