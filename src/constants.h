#ifndef CONSTANTS_H
#define CONSTANTS_H

// Enum to represent the four possible movement directions with unique bit values
typedef enum {
    UP   = 1, // Binary 0001, moves up
    RIGHT= 2, // Binary 0010, moves right
    DOWN = 4, // Binary 0100, moves down
    LEFT = 8  // Binary 1000, moves left
} Direction;

// Boolean-like definitions for TRUE/FALSE
#define TRUE 1
#define FALSE 0

// Rotation direction constants
#define CLOCKWISE -1             // Rotate clockwise
#define COUNTER_CLOCKWISE 1      // Rotate counter-clockwise

// Mathematical constant Pi
#define PI 3.14159

// Window size for rendering
#define WINDOW_HEIGHT 600        // Window height in pixels
#define WINDOW_WIDTH 900         // Window width in pixels

// Player attributes
#define PLAYER_WIDTH  10         // Player width in pixels
#define PLAYER_HEIGHT 10         // Player height in pixels
#define PLAYER_MOVE_SPEED 100    // Movement speed (units per second)
#define PLAYER_ROTATION_SPEED 1.3// Rotation speed (radians per second)
#define PLAYER_JUMP_VELOCITY 0;  // Initial vertical velocity for jumping (currently unused)
#define GRAVITY_ACCELERATION 550;// Gravitational pull, affecting jump mechanics

// Raycasting constants
#define RAYS_NUMBER (WINDOW_WIDTH)  // Number of rays cast, typically equal to screen width
//#define FOV (3.5 * PI / 5)        // Alternative field of view
#define FOV (PI / 3)                // Current field of view (60 degrees)
//#define FOV (2*PI)                // Another alternative FOV covering a full 360 degrees

// Rendering mode
#define FIRST_PERSON 0            // Flag to enable first-person rendering mode

// Mouse sensitivity for camera movement
#define MOUSE_SENSITIVITY 2       // Multiplier for mouse movement

// Environment size constants
#define FLOOR_SIZE (WINDOW_HEIGHT / 2) // Size of the floor area on the screen
#define WALL_SIZE 50                   // Size of a wall in the environment (units)

#endif
