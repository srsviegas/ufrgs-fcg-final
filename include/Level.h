#ifndef LEVEL_H
#define LEVEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <fstream>

#include "matrices.h"
#include "utils.h"
#include "Constants.h"

class Plane
{
public:
    Plane(int tex, glm::vec3 pos, glm::vec3 rot);
    void DebugPrint();

    int texture;        // Texture value
    glm::vec3 position; // Translation coordinates
    glm::vec3 rotation; // Rotation coordinates (in degrees)
    glm::vec4 normal;   // Plane's normal vector
    glm::vec4 side;     // Plane's side vector
    float D;            // Coefficient for plane's eq. (Ax + By + Cz + D = 0)

    bool hasTorch;        // Indicates if that plane should have a torch light
    glm::vec3 torchColor; // The color of the torch, if it has one
};

class Level
{
public:
    Level(uint16_t levelNumber);
    std::vector<std::vector<std::vector<Plane>>> BuildPlaneData();

    std::vector<std::vector<std::vector<Plane>>> GetPlaneData();
    std::vector<Plane> GetPlanesAtTile(int x, int z);
    std::vector<Plane> GetWallsAtTile(int x, int z);
    std::vector<Plane> GetWallsAroundTile(int x, int z);

    bool IsFloor(int x, int z);
    bool IsPlayerSpawn(int x, int z);
    bool ObjectiveReached(glm::vec4 position);
    int WorldPositionToMapPositionX(float x);
    int WorldPositionToMapPositionZ(float z);
    glm::vec4 MapPositionToWorldPosition(int x, int z);

    std::vector<std::string> GetMap();
    int GetMapHeight();
    int GetMapWidth();
    glm::vec4 GetPlayerInitialPosition();
    glm::vec4 GetObjectivePosition();
    glm::vec4 GetCenterWorldPosition();

private:
    std::vector<std::string> mapData;
    int mapHeight;
    int mapWidth;
    glm::vec4 playerInitialPosition;
    glm::vec4 objectivePosition;
    glm::vec4 center;
    std::vector<std::vector<std::vector<Plane>>> planeData;
};

extern std::string mapData1[5];

#endif