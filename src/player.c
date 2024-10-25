// std
#include <math.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "gametime.h"
#include "algebra.h"
#include "player.h"

// Global player object
struct player player;

/**
 * Initializes player properties.
 * Sets initial position, size, and movement attributes.
 */
void setup_player(void) {
    player.width = PLAYER_WIDTH; // Set player width
    player.height = PLAYER_HEIGHT; // Set player height

    player.position.x = WINDOW_WIDTH / 2.0f - player.width / 2.0f; // Center player horizontally
    player.position.y = WINDOW_HEIGHT / 2.0f - player.height / 2.0f; // Center player vertically

    // Initialize velocity, rotation, and angle
    player.velocity.x = 0;
    player.velocity.y = 0;
    player.rotation = 0;
    player.angle = 0;
    player.z_offset = 0;
    player.z_vel = PLAYER_Z_VEL;

    // Initialize movement state (not moving)
    player.move_set.front = FALSE;
    player.move_set.back  = FALSE;
    player.move_set.right = FALSE;
    player.move_set.left  = FALSE;

    // Initialize possible moves (all moves are initially possible)
    player.possible_moves.front = TRUE;
    player.possible_moves.back  = TRUE;
    player.possible_moves.right = TRUE;
    player.possible_moves.left  = TRUE;
}

/**
 * Updates player velocity based on current movement inputs.
 * Calculates direction and speed of movement.
 */
void set_move_player(void) {
    player.velocity.x = 0; // Reset horizontal velocity
    player.velocity.y = 0; // Reset vertical velocity

    // Update velocity based on movement input and direction
    if(player.move_set.right && player.possible_moves.right) {
        player.velocity.x += cos(player.angle + PI/2); // Move right
        player.velocity.y += sin(player.angle + PI/2);
    } else if(player.move_set.left && player.possible_moves.left) {
        player.velocity.x += cos(player.angle - PI/2); // Move left
        player.velocity.y += sin(player.angle - PI/2);
    }

    if(player.move_set.front && player.possible_moves.back) {
        player.velocity.x += cos(player.angle); // Move forward
        player.velocity.y += sin(player.angle);
    } else if(player.move_set.back && player.possible_moves.back) {
        player.velocity.x += cos(player.angle + PI); // Move backward
        player.velocity.y += sin(player.angle + PI);
    }

    // Normalize velocity to ensure consistent movement speed
    normalize_vector2(player.velocity);
    player.velocity.x *= PLAYER_MOVE_SPEED;
    player.velocity.y *= PLAYER_MOVE_SPEED;
}

/**
 * Updates player rotation and return desired position based on elapsed time.
 * 
 * @param delta_time The time elapsed since the last update.
 * @return struct vec2 The players desired position after elapsed time.
 */
struct vec2 update_player(double delta_time) {
    set_move_player(); // Update player velocity based on inputs

    // Update player rotation based on mouse input
    player.angle -= player.rotation * PLAYER_ROTATION_SPEED * delta_time;
    normalize_angle(&player.angle); // Ensure angle is within 0 to 2π

    // players desired location:
    // Update player position based on velocity and elapsed time
    struct vec2 position;
    position.x = player.position.x + player.velocity.x * delta_time;
    position.y = player.position.y + player.velocity.y * delta_time;

    if(player.velocity.x != 0 && player.velocity.y != 0) {
        if(player.z_vel == 0) player.z_vel = PLAYER_Z_VEL;

        player.z_offset += player.z_vel * delta_time;
        if((player.z_offset > MAX_Z_OFFSET && player.z_vel > 0) || (player.z_offset < 0 && player.z_vel < 0))
            player.z_vel = -player.z_vel;
    } else {
        if(player.z_offset > 0) {
            player.z_vel = -PLAYER_Z_VEL;
            player.z_offset += player.z_vel * delta_time;
            if(player.z_offset < 0) player.z_offset = 0;
        } else {
            player.z_offset = 0;

        }
    }

    return position;
}

/**
 * Renders the player on the screen.
 * Draws player as a rectangle and a line indicating facing direction.
 * 
 * @param renderer The renderer used for drawing.
 */
void render_player(SDL_Renderer* renderer) {
    // Create a rectangle representing the player
    SDL_Rect player_rect = {
        (int) (player.position.x - player.width / 2), // X position adjusted by half width
        (int) (player.position.y - player.height / 2), // Y position adjusted by half height
        (int) player.width,
        (int) player.height
    };

    SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255); // Set color to reddish for the player
    SDL_RenderFillRect(renderer, &player_rect); // Draw player rectangle

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set color to green for the direction line
    SDL_RenderDrawLine(
        renderer, 
        (int) player.position.x, 
        (int) player.position.y, 
        (int) (player.position.x + cos(player.angle) * player.width), // End vec2 of direction line based on angle
        (int) (player.position.y + sin(player.angle) * player.width)
    );
}

/**
 * Rotates player towards a specific vec2.
 * Calculates the angle from player to the target vec2.
 * 
 * @param x The x-coordinate of the target vec2.
 * @param y The y-coordinate of the target vec2.
 */
void rotate_player_towards(int x, int y) {
    player.angle = atan2f(y - player.position.y, x - player.position.x); // Compute angle using arctangent
}
