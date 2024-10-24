#include "levels.h"
#include "section.h"

// Map definition: simple 2D array representing lines with their RGB color values
const int map_lines = 17;
double map[100][7] = {
    {10, 10, 300, 10, 255, 0, 0},       // Top horizontal wall
    {10, 10, 10, 300, 255, 0, 0},       // Left vertical wall
    {300, 10, 300, 300, 255, 0, 0},     // Right vertical wall
    // Internal walls
    {50,  10,  50,  100, 0,   255, 0},       // Vertical wall left
    {50,  100, 100, 100, 0,   255, 0},     // Horizontal section
    {100, 100, 100, 200, 0,   255, 0},    // Vertical wall middle left
    {150, 50,  150, 150, 0,   255, 0},     // Vertical wall middle
    {150, 150, 200, 150, 0,   255, 0},    // Horizontal section
    {100, 200, 200, 200, 0,   0,   255},    // Horizontal wall middle bottom
    {200, 200, 200, 250, 0,   0,   255},    // Vertical wall near bottom
    {200, 250, 250, 250, 0,   0,   255},    // Bottom right horizontal section
    {250, 50,  250, 150, 255, 255, 0},   // Vertical wall middle right
    {250, 50,  300, 50,  255, 255, 0},    // Top-right horizontal wall
    {150, 150, 150, 200, 0,   255, 255},  // Vertical middle wall extension
    {200, 50,  150, 50,  0,   255, 255},    // Horizontal upper middle wall
    {50,  250, 150, 250, 0,   255, 255},   // Horizontal lower middle wall
    {50,  150, 50,  200, 255, 0,   255},    // Vertical left-bottom wall
};

// Function to create the first level of the game
// Returns a pointer to the newly created section representing level 1
struct section* create_level_1(void) {
    struct section* level = section_create(2, map_lines);
    struct line wall;
    for(int i = 0; i < map_lines; i++) {
        wall.x0 = map[i][0];
        wall.y0 = map[i][1];
        wall.xf = map[i][2];
        wall.yf = map[i][3];
        section_add_wall(level, wall);
    }

    return level;
}