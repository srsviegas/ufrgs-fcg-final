#ifndef LEVEL_H
#define LEVEL_H

#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3
#define HUD_HEALTH 4
#define HUD_MANA 5
#define SPHERE 7

#define ENEMY_TYPE_1 12

#define NUM_PROJECTILES 10

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

struct AABB
{
    glm::vec3 AABB_min;
    glm::vec3 AABB_max;
};

struct Plane
{
    int texture;       // Texture value
    float position[3]; // Translation coordinates
    float rotation[3]; // Rotation (in degrees)
};

extern std::string mapData[5];

std::vector<Plane> BuildLevelData(int height, int width, std::string *map);

#endif