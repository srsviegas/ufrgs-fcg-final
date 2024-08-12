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

    void DebugPrint()
    {
        printf("T(%d)\t P(%f, %f, %f)\t R(%f, %f, %f)\n",
               texture,
               position[0], position[1], position[2],
               rotation[0], rotation[1], rotation[2]);
    }
};

class Level
{
public:
    Level(std::string *map, int numRows);
    std::vector<std::vector<std::vector<Plane>>> BuildPlaneData();

    std::vector<std::vector<std::vector<Plane>>> GetPlaneData();
    std::vector<Plane> GetPlanesAtTile(int x, int z);

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