#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

/*
    Structure to hold player movement state.
    This includes whether the player is moving in specific directions or jumping.
*/
typedef struct {
    int front;  // 1 if moving forward, 0 otherwise
    int back;   // 1 if moving backward, 0 otherwise
    int right;  // 1 if moving right, 0 otherwise
    int left;   // 1 if moving left, 0 otherwise
    int jump;   // 1 if the player is jumping, 0 otherwise
} MoveSet;

/*
    Structure to represent a player in the game.
    This includes the player's position, velocity, dimensions, and movement states.
*/
typedef struct {
    double rotation;      // The current rotation of the player (angle)
    double x;             // The x-coordinate of the player's position
    double y;             // The y-coordinate of the player's position
    double z;             // The z-coordinate (height) of the player's position
    double z_vel;         // The vertical velocity (used for jumping and gravity)
    int is_jumping;      // 1 if the player is in the air (jumping), 0 otherwise
    double width;         // The width of the player (used for rendering)
    double height;        // The height of the player (used for rendering)
    double velocity[2];   // The velocity vector of the player (x and y components)
    double angle;         // The angle the player is facing
    MoveSet move_set;    // The current movement states (front, back, right, left, jump)
    MoveSet possible_moves; // The possible moves based on the environment (obstacles, etc.)
} Player;

/*
    Updates the player's state based on the elapsed time.
    This function calculates the new position and velocity of the player,
    considering factors such as movement input, jumping, and gravity.
    Parameters:
        - double delta_time: The time elapsed since the last update, used for time-based calculations
*/
void update_player(double delta_time);

/*
    Renders the player on the screen using the provided renderer.
    This function draws the player as a rectangle and an additional line to show the direction the player is facing.
    Parameters:
        - SDL_Renderer* renderer: The renderer used to draw the player on the screen
*/
void render_player(SDL_Renderer* renderer);

/*
    Initializes the player's properties.
    This function sets up the player's position, dimensions, movement states, and other initial values.
*/
void setup_player(void);

/*
    Calculates and updates the player's velocity based on current movement inputs.
    This function adjusts the player's velocity according to the direction of movement and other factors like jumping.
*/
void set_move_player();

/*
    Rotates the player to face a target point specified by the mouse coordinates.
    This function adjusts the player's facing direction based on the position of the mouse cursor.
    Parameters:
        - int x: The x-coordinate of the target point (mouse position)
        - int y: The y-coordinate of the target point (mouse position)
*/
void rotate_player_towards(int x, int y);

#endif