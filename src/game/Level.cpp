#include "Level.h"

std::vector<Plane> levelData = {
    // Floors
    {TXT_FLOOR, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},

    // Walls
    {TXT_WALL, {-1.0f, 0.0f,  0.0f}, { 90.0f, 0.0f, -90.0f}},
    {TXT_WALL, { 0.0f, 0.0f, -1.0f}, { 90.0f, 0.0f,   0.0f}},
    {TXT_WALL, { 1.0f, 0.0f,  0.0f}, { 90.0f, 0.0f,  90.0f}},
    {TXT_WALL, { 0.0f, 0.0f,  1.0f}, { 90.0f, 0.0f, 180.0f}},

    // Ceilings
    {TXT_CEIL, {0.0f, 1.0f, 0.0f}, {180.0f, 0.0f, 0.0f}},
};
