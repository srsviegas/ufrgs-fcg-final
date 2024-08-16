#include "Level.h"

std::string mapData1[5] = {
    "XXXXXX",
    "X   XX",
    "X  XXX",
    "X XXpX",
    "XXXXXX",
};

glm::vec3 torchColors[3] = {
    glm::vec3(1.0f, 0.25f, 0.01f),
    glm::vec3(1.0f, 0.15f, 0.01f),
    glm::vec3(1.0f, 0.05f, 0.01f)};

Plane::Plane(int tex, glm::vec3 pos, glm::vec3 rot)
{
    texture = tex;
    position = pos;
    rotation = rot;
    hasTorch = (pos.y == 0.0f) ? chance(TORCH_CHANCE_PER_WALL) : false;

    if (hasTorch)
    {
        torchColor = torchColors[random(3)];
    }

    glm::mat4 transformation = Matrix_Translate(position.x, position.y, position.z);
    transformation *= Matrix_Rotate_X(glm::radians(rotation.x));
    transformation *= Matrix_Rotate_Y(glm::radians(rotation.y));
    transformation *= Matrix_Rotate_Z(glm::radians(rotation.z));

    glm::vec4 a = transformation * glm::vec4({1.0f, 0.0f, 1.0f, 1.0f});
    glm::vec4 b = transformation * glm::vec4({-1.0f, 0.0f, -1.0f, 1.0f});
    glm::vec4 c = transformation * glm::vec4({-1.0f, 0.0f, 1.0f, 1.0f});

    normal = glm::normalize(crossproduct(b - a, c - a));
    D = -glm::dot(normal, a);
}

void Plane::DebugPrint()
{
    printf("T(%d)\t P(%f, %f, %f)\t R(%f, %f, %f)\n",
           texture,
           position.x, position.y, position.z,
           rotation.x, rotation.y, rotation.z);
}

/* Builds a level from a map in format of an array of strings. The 'X' char represents
a tiled floor, a ' ' (space) char represents an empty floor tile and a 'p' char represents
the initial player position.

A map with `height = 3` and `width = 6` can be like the following:
```
"XXXXXX"
" X   "
"XXXpX"
``` */
Level::Level(std::string *map, int numRows)
{
    mapData = map;
    mapHeight = numRows;
    mapWidth = 0;
    int x, z = 0;

    // Calculates map width
    for (int i = 0; i < mapHeight; i++)
    {
        int lineWidth = mapData[i].length();
        if (lineWidth > mapWidth)
        {
            mapWidth = lineWidth;
        }
    }

    // Searches for player initial position inside map
    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            if (mapData[i][j] == 'p')
            {
                x = j;
                z = i;
                mapData[i][j] = 'X';
            }
        }
    }
    printf("Player Initial Position: (%d, %d)\n", x, z);
    playerInitialPosition = glm::vec4({(float)x, 0.0f, float(z), 1.0f});

    planeData = BuildPlaneData();
}

/* Builds the plane transformation for each floor tile, wall tile and ceiling tile from
level data. */
std::vector<std::vector<std::vector<Plane>>> Level::BuildPlaneData()
{
    std::vector<std::vector<std::vector<Plane>>> planes(GetMapHeight(), std::vector<std::vector<Plane>>(GetMapWidth()));

    // Create planes for each tile in map
    for (int i = 0; i < mapHeight; i++)
    {
        // Create planes for each tile in row i and push it inside planes vector
        for (int j = 0; j < mapWidth; j++)
        {
            if (mapData[i][j] == 'X')
            {
                float x = ((float)j - playerInitialPosition.x) * 2.0f;
                float z = ((float)i - playerInitialPosition.z) * 2.0f;

                // Create plane for tile's floor
                planes[i][j].push_back(Plane(TXT_FLOOR, {x, -1.0f, z}, {0.0f, 0.0f, 0.0f}));

                // Create plane for tile's ceiling
                planes[i][j].push_back(Plane(TXT_CEIL, {x, 1.0f, z}, {180.0f, 0.0f, 0.0f}));

                // Create planes for tile's walls
                if (i == 0 || mapData[i - 1][j] == ' ')
                {
                    // i - i wall
                    planes[i][j].push_back(Plane(TXT_WALL, {x, 0.0f, z - 1}, {90.0f, 0.0f, 0.0f}));
                }
                if (i == mapHeight - 1 || mapData[i + 1][j] == ' ')
                {
                    // i + 1 wall
                    planes[i][j].push_back(Plane(TXT_WALL, {x, 0.0f, z + 1}, {90.0f, 0.0f, 180.0f}));
                }
                if (j == 0 || mapData[i][j - 1] == ' ')
                {
                    // j - 1 wall
                    planes[i][j].push_back(Plane(TXT_WALL, {x - 1, 0.0f, z}, {90.0f, 0.0f, -90.0f}));
                }
                if (j == mapWidth - 1 || mapData[i][j + 1] == ' ')
                {
                    // j + 1 wall
                    planes[i][j].push_back(Plane(TXT_WALL, {x + 1, 0.0f, z}, {90.0f, 0.0f, 90.0f}));
                }
            }
        }
    }

    return planes;
}

std::vector<std::vector<std::vector<Plane>>> Level::GetPlaneData()
{
    return planeData;
}

/* Gets all plane coordinates associated with the tile (x, z). If a non-empty vector is
returned, the first plane will be the tile's floor, the second will be the tile's ceiling,
and the others will be the tile's walls (max. 4). */
std::vector<Plane> Level::GetPlanesAtTile(int x, int z)
{
    if (z < 0 || z >= GetMapHeight() || x < 0 || x >= GetMapWidth())
    {
        // (x, z) not out of bounds; returns an empty vector
        return std::vector<Plane>();
    }

    return GetPlaneData()[z][x];
}

/* Gets all plane coordinates for walls associated with the tile (x, y). If there are no
walls, an empty vector will be returned. */
std::vector<Plane> Level::GetWallsAtTile(int x, int z)
{
    std::vector<Plane> planes = GetPlanesAtTile(x, z);

    if (planes.size() <= 2)
    {
        // There are no walls at (x, z)
        return std::vector<Plane>();
    }

    std::vector<Plane> walls(planes.begin() + 2, planes.end());

    return walls;
}

/* Get all plane coordinates for walls associated to the tiles in a square around the tile
(x, y). For example, `GetWallsAroundTile(1, 1)` would return all walls related to tiles
(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 0), and so on. */
std::vector<Plane> Level::GetWallsAroundTile(int x, int z)
{
    std::vector<Plane> wallsAroundTile;

    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            std::vector<Plane> wallsAtTile = GetWallsAtTile(x + i, z + j);
            wallsAroundTile.insert(wallsAroundTile.end(), wallsAtTile.begin(), wallsAtTile.end());
        }
    }

    return wallsAroundTile;
}

bool Level::IsFloor(int x, int z)
{
    bool xInBounds = (x >= 0) && (x < mapWidth);
    bool yInBounds = (z >= 0) && (z < mapHeight);
    return xInBounds && yInBounds && (mapData[z][x] == 'X');
}

int Level::WorldPositionToMapPositionX(float x)
{
    return (int)(floor((x + 1.0f) / 2.0f + playerInitialPosition.x));
}

int Level::WorldPositionToMapPositionZ(float z)
{
    return (int)(floor((z + 1.0f) / 2.0f + playerInitialPosition.z));
}

int Level::GetMapHeight()
{
    return mapHeight;
}

int Level::GetMapWidth()
{
    return mapWidth;
}

glm::vec4 Level::GetPlayerInitialPosition()
{
    return playerInitialPosition;
}