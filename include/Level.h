#ifndef LEVEL_H
#define LEVEL_H

#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

struct Plane
{
    int texture;       // Texture value
    float position[3]; // Translation coordinates
    float rotation[3]; // Rotation (in degrees)
};

extern std::vector<Plane> levelData;

#endif