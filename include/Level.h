#ifndef LEVEL_H
#define LEVEL_H

#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3
#define HUD_HEALTH 4
#define HUD_MANA 5
#define HUD_MAPTILE 6
#define SPHERE 7

#define ENEMY_TYPE_1 12

#define NUM_PROJECTILES 10

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

struct Plane
{
    int texture;       // Texture value
    float position[3]; // Translation coordinates
    float rotation[3]; // Rotation (in degrees)
};

class Level
{
public:
    Level(std::string *map, int numRows);
    std::vector<Plane> BuildPlaneData();

    bool IsFloor(int x, int y);
    int GetMapHeight();
    int GetMapWidth();
    glm::vec4 GetPlayerInitialPosition();

private:
    std::string *mapData;
    int mapHeight;
    int mapWidth;
    glm::vec4 playerInitialPosition;
};

extern std::string mapData1[5];

#endif