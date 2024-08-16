#ifndef LEVEL_H
#define LEVEL_H

#define TXT_FLOOR 1
#define TXT_WALL 2
#define TXT_CEIL 3
#define HUD_HEALTH 4
#define HUD_MANA 5
#define HUD_MAPTILE 6
#define PROJECTILE_WATER 7
#define LEFT_ARM 8
#define RIGHT_ARM 9
#define TORCH 10

#define ENEMY_TYPE_1 12

#define NUM_PROJECTILES 10

#define TORCH_CHANCE_PER_WALL 40

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "matrices.h"
#include "utils.h"

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

    bool hasTorch;        // Indicates if that plane should have a torch light
    glm::vec3 torchColor; // The color of the torch, if it has one
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