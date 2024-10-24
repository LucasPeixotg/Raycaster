#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "algebra.h"

/*
    Structure to hold player movement state.
    This includes whether the player is moving in specific directions or jumping.
*/
struct move_set {
    int front;  // 1 if moving forward, 0 otherwise
    int back;   // 1 if moving backward, 0 otherwise
    int right;  // 1 if moving right, 0 otherwise
    int left;   // 1 if moving left, 0 otherwise
};

/*
    Structure to represent a player in the game.
    This includes the player's position, velocity, dimensions, and movement states.
*/
struct player {
    double rotation;      // The current rotation of the player (angle)
    struct point position;             // The y-coordinate of the player's position
    double z_vel;         // The vertical velocity (used for jumping and gravity)
    double width;         // The width of the player (used for rendering)
    double height;        // The height of the player (used for rendering)
    double velocity[2];   // The velocity vector of the player (x and y components)
    double angle;         // The angle the player is facing
    struct move_set move_set;    // The current movement states (front, back, right, left, jump)
    struct move_set possible_moves; // The possible moves based on the environment (obstacles, etc.)
};

/**
 * Initializes player properties.
 * Sets initial position, size, and movement attributes.
 */
void setup_player(void);

/**
 * Updates player rotation and return desired position based on elapsed time.
 * 
 * @param delta_time The time elapsed since the last update.
 * @return struct point The players desired position after elapsed time.
 */
struct point update_player(double delta_time);


/**
 * Renders the player on the screen.
 * Draws player as a rectangle and a line indicating facing direction.
 * 
 * @param renderer The renderer used for drawing.
 */
void render_player(SDL_Renderer* renderer);


/**
 * Updates player velocity based on current movement inputs.
 * Calculates direction and speed of movement.
 */
void set_move_player();

/**
 * Rotates player towards a specific point.
 * Calculates the angle from player to the target point.
 * 
 * @param x The x-coordinate of the target point.
 * @param y The y-coordinate of the target point.
 */
void rotate_player_towards(int x, int y);

#endif