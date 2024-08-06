#include "Level.h"

std::string mapData[5] = {
    "XXXXXX",
    "X   XX",
    "X  XXX",
    "X XXpX",
    "XXXXXX",
};

/* Builds level data from a map in format of an array of strings. The 'X' char represents
a tiled floor, a ' ' (space) char represents an empty floor tile and a 'p' char represents
the initial player position.

A map with `height = 3` and `width = 6` can be like the following:
```
"XXXXXX"
" X   "
"XXXpX"
``` */
std::vector<Plane> BuildLevelData(int height, int width, std::string *map)
{
    std::vector<Plane> levelData;
    float px, py = 0.0f;

    // Search for player initial position in map
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == 'p')
            {
                px = (float)j;
                py = (float)i;
                map[i][j] = 'X';
            }
        }
    }

    // Create planes for each tile in map
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == 'X')
            {
                float x = ((float)j - px) * 2.0f;
                float y = ((float)i - py) * 2.0f;

                // Create plane for tile's floor
                levelData.push_back({TXT_FLOOR, {x, -1.0f, y}, {0.0f, 0.0f, 0.0f}});

                // Create plane for tile's ceiling
                levelData.push_back({TXT_CEIL, {x, 1.0f, y}, {180.0f, 0.0f, 0.0f}});

                // Create planes for tile's walls
                if (i == 0 || map[i - 1][j] == ' ')
                {
                    // i - i wall
                    levelData.push_back({TXT_WALL, {x, 0.0f, y - 1}, {90.0f, 0.0f, 0.0f}});
                }
                if (i == height - 1 || map[i + 1][j] == ' ')
                {
                    // i + 1 wall
                    levelData.push_back({TXT_WALL, {x, 0.0f, y + 1}, {90.0f, 0.0f, 180.0f}});
                }
                if (j == 0 || map[i][j - 1] == ' ')
                {
                    // j - 1 wall
                    levelData.push_back({TXT_WALL, {x - 1, 0.0f, y}, {90.0f, 0.0f, -90.0f}});
                }
                if (j == width - 1 || map[i][j + 1] == ' ')
                {
                    // j + 1 wall
                    levelData.push_back({TXT_WALL, {x + 1, 0.0f, y}, {90.0f, 0.0f, 90.0f}});
                }
            }
        }
    }

    return levelData;
}