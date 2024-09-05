// std
#include <math.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "gametime.h"
#include "utils.h"
#include "player.h"

// Global player object
Player player;

/*
    Initializes player properties.
    Sets initial position, size, and movement attributes.
*/
void setup_player(void) {
    player.width = PLAYER_WIDTH; // Set player width
    player.height = PLAYER_HEIGHT; // Set player height

    player.x = WINDOW_WIDTH / 2.0f - player.width / 2.0f; // Center player horizontally
    player.y = WINDOW_HEIGHT / 2.0f - player.height / 2.0f; // Center player vertically
    player.z = 0; // Initialize vertical position to 0 (on the ground)

    // Initialize velocity, rotation, and angle
    player.velocity[0] = 0;
    player.velocity[1] = 0;
    player.rotation = 0;
    player.angle = 0;

    // Initialize movement state (not moving)
    player.move_set.front = FALSE;
    player.move_set.back  = FALSE;
    player.move_set.right = FALSE;
    player.move_set.left  = FALSE;
    player.move_set.jump  = FALSE;

    // Initialize possible moves (all moves are initially possible)
    player.possible_moves.front    = TRUE;
    player.possible_moves.back     = TRUE;
    player.possible_moves.right    = TRUE;
    player.possible_moves.left     = TRUE;
    player.possible_moves.jump     = TRUE;
}

/*
    Updates player velocity based on current movement inputs.
    Calculates direction and speed of movement.
*/
void set_move_player(void) {
    player.velocity[0] = 0; // Reset horizontal velocity
    player.velocity[1] = 0; // Reset vertical velocity

    // Update velocity based on movement input and direction
    if(player.move_set.right && player.possible_moves.right) {
        player.velocity[0] += cos(player.angle + PI/2); // Move right
        player.velocity[1] += sin(player.angle + PI/2);
    } else if(player.move_set.left && player.possible_moves.left) {
        player.velocity[0] += cos(player.angle - PI/2); // Move left
        player.velocity[1] += sin(player.angle - PI/2);
    }

    if(player.move_set.front && player.possible_moves.back) {
        player.velocity[0] += cos(player.angle); // Move forward
        player.velocity[1] += sin(player.angle);
    } else if(player.move_set.back && player.possible_moves.back) {
        player.velocity[0] += cos(player.angle + PI); // Move backward
        player.velocity[1] += sin(player.angle + PI);
    }

    // Handle jumping
    if(player.is_jumping && player.z == 0) {
        player.possible_moves.jump = TRUE; // Allow jumping again if on the ground
        player.is_jumping = FALSE;
    }

    if(player.move_set.jump && player.possible_moves.jump) {
        player.z_vel = PLAYER_JUMP_VELOCITY; // Set jump velocity
        player.possible_moves.jump = FALSE; // Disable further jumps until landing
        player.is_jumping = TRUE;
    }

    // Normalize velocity to ensure consistent movement speed
    normalize_vector2(player.velocity);
    player.velocity[0] *= PLAYER_MOVE_SPEED;
    player.velocity[1] *= PLAYER_MOVE_SPEED;
}

/*
    Updates player position and state based on elapsed time.
    Applies gravity and adjusts movement and rotation.
    Parameters:
        - double delta_time: time elapsed since last update
*/
void update_player(double delta_time) {
    set_move_player(); // Update player velocity based on inputs

    player.z_vel -= delta_time * GRAVITY_ACCELERATION; // Apply gravity to vertical velocity

    player.z += player.z_vel * delta_time; // Update vertical position
    if(player.z < 0 ) player.z = 0; // Ensure player doesn't go below ground

    if(player.is_jumping) {
        player.velocity[0] *= 1.5; // Increase velocity when jumping
        player.velocity[1] *= 1.5;
    }

    // Update player position based on velocity and elapsed time
    player.x += player.velocity[0] * delta_time;
    player.y += player.velocity[1] * delta_time;

    // Update player rotation based on mouse input
    player.angle -= player.rotation * PLAYER_ROTATION_SPEED * delta_time;
    normalize_angle(&player.angle); // Ensure angle is within 0 to 2π
}

/*
    Renders the player on the screen.
    Draws player as a rectangle and a line indicating facing direction.
    Parameters:
        - SDL_Renderer* renderer: the renderer used for drawing
*/
void render_player(SDL_Renderer* renderer) {
    // Create a rectangle representing the player
    SDL_Rect player_rect = {
        (int) (player.x - player.width / 2), // X position adjusted by half width
        (int) (player.y - player.height / 2), // Y position adjusted by half height
        (int) player.width,
        (int) player.height
    };

    SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255); // Set color to reddish for the player
    SDL_RenderFillRect(renderer, &player_rect); // Draw player rectangle

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set color to green for the direction line
    SDL_RenderDrawLine(
        renderer, 
        (int) player.x, 
        (int) player.y, 
        (int) (player.x + cos(player.angle) * player.width), // End point of direction line based on angle
        (int) (player.y + sin(player.angle) * player.width)
    );
}

/*
    Rotates player towards a specific point.
    Calculates the angle from player to the target point.
    Parameters:
        - int x: x-coordinate of the target point
        - int y: y-coordinate of the target point
*/
void rotate_player_towards(int x, int y) {
    player.angle = atan2f(y - player.y, x - player.x); // Compute angle using arctangent
}