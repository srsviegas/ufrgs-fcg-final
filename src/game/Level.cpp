#include "Level.h"

std::string mapData[5] = {
    "XXXXXX",
    "X   XX",
    "X  XXX",
    "X XXXX",
    "XXXXXX",
};

/* Builds level data from a map in format of an array of strings. The 'X' char represents
a tiled floor, and a ' ' (space) char represents an empty floor tile.

A map with `height = 3` and `width = 6` can be like the following:
```
"XXXXXX"
" X   "
"XXXXX"
``` */
std::vector<Plane> BuildLevelData(int height, int width, std::string *map)
{
    std::vector<Plane> levelData;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == 'X')
            {
                float x = (float)i * 2;
                float y = (float)j * 2;

                // Create plane for tile's floor
                levelData.push_back({TXT_FLOOR, {x, -1.0f, y}, {0.0f, 0.0f, 0.0f}});

                // Create plane for tile's ceiling
                levelData.push_back({TXT_CEIL, {x, 1.0f, y}, {180.0f, 0.0f, 0.0f}});

                // Create planes for tile's walls
                if (i == 0 || map[i - 1][j] == ' ')
                {
                    // i - i wall
                    levelData.push_back({TXT_WALL, {x - 1, 0.0f, y}, {90.0f, 0.0f, -90.0f}});
                }
                if (i == height - 1 || map[i + 1][j] == ' ')
                {
                    // i + 1 wall
                    levelData.push_back({TXT_WALL, {x + 1, 0.0f, y}, {90.0f, 0.0f, 90.0f}});
                }
                if (j == 0 || map[i][j - 1] == ' ')
                {
                    // j - 1 wall
                    levelData.push_back({TXT_WALL, {x, 0.0f, y - 1}, {90.0f, 0.0f, 0.0f}});
                }
                if (j == width - 1 || map[i][j + 1] == ' ')
                {
                    // j + 1 wall
                    levelData.push_back({TXT_WALL, {x, 0.0f, y + 1}, {90.0f, 0.0f, 180.0f}});
                }
            }
        }
    }

    return levelData;
}