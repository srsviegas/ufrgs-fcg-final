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

#include "matrices.h"

class Plane
{
public:
    Plane(int tex, glm::vec3 pos, glm::vec3 rot);
    void DebugPrint();

    int texture;        // Texture value
    glm::vec3 position; // Translation coordinates
    glm::vec3 rotation; // Rotation coordinates (in degrees)
    glm::vec4 normal;   // Plane's normal vector
    float D;            // Coefficient for plane's eq. (Ax + By + Cz + D = 0)
};

class Level
{
public:
    Level(std::string *map, int numRows);
    std::vector<std::vector<std::vector<Plane>>> BuildPlaneData();

    std::vector<std::vector<std::vector<Plane>>> GetPlaneData();
    std::vector<Plane> GetPlanesAtTile(int x, int z);
    std::vector<Plane> GetWallsAtTile(int x, int z);
    std::vector<Plane> GetWallsAroundTile(int x, int z);

    bool IsFloor(int x, int z);
    int WorldPositionToMapPositionX(float x);
    int WorldPositionToMapPositionZ(float z);

    int GetMapHeight();
    int GetMapWidth();
    glm::vec4 GetPlayerInitialPosition();

private:
    std::string *mapData;
    int mapHeight;
    int mapWidth;
    glm::vec4 playerInitialPosition;
    std::vector<std::vector<std::vector<Plane>>> planeData;
};

extern std::string mapData1[5];

#endif