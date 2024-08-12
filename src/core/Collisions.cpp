#include "Collisions.h"

/* Creates a Sphere object, where `c` is the center position as a vec3 and `r` is the
radius as a float value. */
Sphere::Sphere(glm::vec3 c, float r)
{
    center = c;
    radius = r;
}

Sphere::Sphere(glm::vec4 c, float r)
{
    center = glm::vec3(c);
    radius = r;
}

/* Checks collision between the Sphere object and an AABB object. */
bool Sphere::IsCollidingAABB(AABB aabb)
{
    glm::vec3 closest;
    closest.x = std::max(aabb.min.x, std::min(center.x, aabb.max.x));
    closest.y = std::max(aabb.min.y, std::min(center.y, aabb.max.y));
    closest.z = std::max(aabb.min.z, std::min(center.z, aabb.max.z));

    glm::vec3 distance = closest - center;
    float distanceSquared = glm::dot(distance, distance);

    return distanceSquared <= (radius * radius);
}