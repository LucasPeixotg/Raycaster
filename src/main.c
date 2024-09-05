// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> // SDL library for graphics

#include "constants.h" // Project constants (screen size, player attributes, etc.)
#include "player.h"    // Player structure and functions
#include "utils.h"     // Utility functions for the game
#include "gametime.h"  // Time handling functions

// Map definition: simple 2D array representing lines with their RGB color values
const int map_lines = 27; 
float map[100][7] = {
    // Horizontal lines
    {10, 10, 110, 10, 255, 0, 0},    // Line 1: Horizontal red line (top boundary)
    {10, 110, 110, 110, 255, 0, 0},  // Line 2: Horizontal red line (bottom boundary)
    
    // Vertical lines
    {10, 10, 10, 110, 0, 255, 0},    // Line 3: Vertical green line (left boundary)
    {110, 10, 110, 110, 0, 255, 0},  // Line 4: Vertical green line (right boundary)
    
    // Diagonal lines
    {20, 20, 80, 80, 0, 0, 255},     // Line 5: Blue diagonal from (20, 20) to (80, 80)
    {80, 20, 20, 80, 0, 0, 255},     // Line 6: Blue diagonal from (80, 20) to (20, 80)
    
    // Complex shapes
    {120, 20, 180, 20, 255, 255, 0}, // Line 7: Yellow horizontal line
    {180, 20, 180, 80, 255, 255, 0}, // Line 8: Yellow vertical line
    {180, 80, 120, 80, 255, 255, 0}, // Line 9: Yellow horizontal line
    {120, 80, 120, 20, 255, 255, 0}, // Line 10: Yellow vertical line
    
    // L-shape structure
    {50, 150, 150, 150, 255, 0, 255},// Line 11: Magenta horizontal
    {150, 150, 150, 200, 255, 0, 255},// Line 12: Magenta vertical

    // Maze-like pattern
    {60, 120, 60, 180, 0, 255, 255}, // Line 13: Cyan vertical line (maze part)
    {60, 180, 120, 180, 0, 255, 255},// Line 14: Cyan horizontal line
    {120, 180, 120, 130, 0, 255, 255},// Line 15: Cyan vertical line (maze part)

    // Cross structure
    {130, 60, 190, 60, 128, 128, 128}, // Line 16: Gray horizontal line
    {160, 30, 160, 90, 128, 128, 128}, // Line 17: Gray vertical line

    // Outer box
    {200, 200, 300, 200, 100, 100, 255}, // Line 18: Blue horizontal
    {300, 200, 300, 300, 100, 100, 255}, // Line 19: Blue vertical
    {300, 300, 200, 300, 100, 100, 255}, // Line 20: Blue horizontal
    {200, 300, 200, 200, 100, 100, 255}, // Line 21: Blue vertical

    // Open spaces
    {250, 50, 300, 50, 255, 165, 0},   // Line 22: Orange horizontal line
    {300, 50, 300, 100, 255, 165, 0},  // Line 23: Orange vertical line
    {250, 100, 300, 100, 255, 165, 0}, // Line 24: Orange horizontal line

    // Additional random lines
    {50, 250, 100, 250, 255, 192, 203}, // Line 25: Pink horizontal line
    {100, 250, 100, 290, 255, 192, 203},// Line 26: Pink vertical line

    // Other structures
    {210, 210, 290, 230, 70, 130, 180} // Line 27: Steel blue diagonal line

    // Remaining lines (empty for now)
    // Continue adding lines if needed...
};

// External variables defined in player.h
extern Player player;

// Global variable to track the game state (running or not)
int game_is_running = FALSE;

/* 
    Function to initialize SDL, create a window, and create a renderer.
    Parameters: 
        - SDL_Window** window: pointer to the window variable
        - SDL_Renderer** renderer: pointer to the renderer variable
    Returns: 
        - TRUE (1) if initialization was successful,
        - FALSE (0) if initialization failed.
*/
int initialize_window(SDL_Window** window, SDL_Renderer** renderer) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) { // Initialize SDL with all subsystems
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

    // Create a window with given size, centered on screen, borderless
    *window = SDL_CreateWindow(
        NULL, 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS // No window frame
    );

    if(!*window) { // Check if window creation failed
        fprintf(stderr, "Error creating SDL window.\n");
        return FALSE;
    }

    // Create a renderer to draw inside the window (using default driver)
    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if(!*renderer) { // Check if renderer creation failed
        fprintf(stderr, "Error creating SDL renderer.\n");
        return FALSE;
    }
    
    // If in first-person mode, lock the mouse and enable blending mode
    if(FIRST_PERSON) {
        SDL_SetRelativeMouseMode(SDL_TRUE); // Hide cursor and track relative mouse movement
        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND); // Enable blending for transparency
    }

    return TRUE; // Initialization succeeded
}

// Setup function to initialize game objects like the player
void setup(void) {
    setup_player(); // Initialize player properties (position, speed, etc.)
}

/* 
    Function to process user input events from SDL.
    Handles keyboard and mouse events to control player movement and actions.
*/
void process_inputs() {
    SDL_Event event; // SDL event structure
    SDL_PollEvent(&event); // Poll for events (non-blocking)
    
    static int mouse_x = 0, mouse_y = 0; // Static variables to track mouse position
    
    if(!FIRST_PERSON) { // If not in first-person mode, get mouse position for rotating player
        SDL_GetMouseState(&mouse_x, &mouse_y);
        rotate_player_towards(mouse_x, mouse_y); // Rotate player towards mouse position
    }

    // Handle different types of events (e.g., keypress, quit)
    switch(event.type) {
        case SDL_QUIT: // Quit event (e.g., clicking the close button)
            game_is_running = FALSE; // Set game running flag to false, exit the game loop
            break;
        case SDL_KEYDOWN: // Key press event
            // Handle specific key actions (e.g., movement, jump, reset)
            if(event.key.keysym.sym == SDLK_ESCAPE) game_is_running = FALSE; // ESC quits game
            if(event.key.keysym.sym == SDLK_w) player.move_set.front = TRUE;  // W moves player forward
            if(event.key.keysym.sym == SDLK_d) player.move_set.right = TRUE;  // D moves player right
            if(event.key.keysym.sym == SDLK_s) player.move_set.back = TRUE;   // S moves player back
            if(event.key.keysym.sym == SDLK_a) player.move_set.left = TRUE;   // A moves player left
            if(event.key.keysym.sym == SDLK_SPACE) player.move_set.jump = TRUE; // Space makes the player jump
            if(event.key.keysym.sym == SDLK_r) setup_player(); // R resets player position

            break;
        case SDL_KEYUP: // Key release event (stop movement when key is released)
            if(event.key.keysym.sym == SDLK_w) player.move_set.front = FALSE;
            if(event.key.keysym.sym == SDLK_d) player.move_set.right = FALSE;
            if(event.key.keysym.sym == SDLK_s) player.move_set.back = FALSE;
            if(event.key.keysym.sym == SDLK_a) player.move_set.left = FALSE;
            if(event.key.keysym.sym == SDLK_SPACE) player.move_set.jump = FALSE;
            break;

        case SDL_MOUSEMOTION: // Mouse movement event
            if(FIRST_PERSON) { // In first-person mode, use relative mouse movement for rotation
                SDL_GetRelativeMouseState(&mouse_x, &mouse_y); // Get mouse motion since last frame
                player.rotation = -mouse_x * MOUSE_SENSITIVITY; // Apply sensitivity scaling to rotation
            }
            break;
        default:
            player.rotation = 0; // Reset rotation if no mouse motion
    }
}

/* 
    Update game state based on time elapsed (delta time).
    Mainly updates player position and movement.
*/
void update() {
    double delta = get_delta_time(); // Get time elapsed since last frame
    update_player(delta); // Update player position and physics based on input and delta time
}

/* 
    Renders the 2D top-down map showing lines from the map array.
    Only renders if not in first-person mode.
*/
void render_map(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white for lines
    for(int i = 0; i < map_lines; i++) { // Loop through all lines in the map
        SDL_RenderDrawLine(renderer, map[i][0], map[i][1], map[i][2], map[i][3]); // Draw each line
    }
}

/* 
    Renders the camera (3D view) using raycasting.
    Casts rays from the player's viewpoint, calculates intersections with walls, 
    and renders vertical slices representing walls.
*/
void render_camera(SDL_Renderer* renderer) {
    float intersection[2]; // Array to store intersection points
    float angle_off =  FOV / RAYS_NUMBER; // Calculate angle step for each ray

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set draw color for rays
    float smallest_intersection[4] = { 0 , 0 , INFINITY, player.angle}; // Track closest intersection
    float angle, height, distance;
    float plane_vector[2] = {
        cos(player.angle + PI/2), // Vector perpendicular to player's view direction
        sin(player.angle + PI/2)
    };

    // Cast rays to detect walls
    for(int i = 0; i < RAYS_NUMBER; i++) {
        int wall_index = 0; // Index of the closest wall for this ray
        for(int j = 0; j < map_lines; j++) { // Loop through all walls in the map
            angle = player.angle + (FOV/2) - (angle_off * i); // Calculate ray angle
            normalize_angle(&angle); // Ensure angle is within 0 to 2*PI
            if(intersection_lines(angle, player.x, player.y, map[j], intersection)) { // Check if ray hits a wall
                distance = distance_from_plane(plane_vector, player.x - intersection[0], player.y - intersection[1]); // Calculate perpendicular distance to wall
                if(distance < smallest_intersection[2]) { // Track the closest intersection
                    smallest_intersection[0] = intersection[0];
                    smallest_intersection[1] = intersection[1];
                    smallest_intersection[2] = distance;
                    smallest_intersection[3] = angle;
                    wall_index = j; // Store index of the wall hit
                }
            }
        }

        // If an intersection was found, render the wall slice
        if(smallest_intersection[2] != INFINITY) {
            float color = smallest_intersection[2] > 600 ? 0.01 : (1 - smallest_intersection[2] / 600); // Diminish brightness with distance

            if(FIRST_PERSON) {
                SDL_SetRenderDrawColor(renderer, map[wall_index][4]*color, map[wall_index][5]*color, map[wall_index][6]*color, 255); // Set wall color
                height = WINDOW_HEIGHT / (smallest_intersection[2] / WALL_SIZE); // Calculate wall height

                // Calculate vertical position of the wall slice
                int yi = WINDOW_HEIGHT - FLOOR_SIZE - height / 2;
                float jump_offset = + 0.7 * player.z * cos(((i - WINDOW_WIDTH/8) * FOV / WINDOW_WIDTH) / 4); // Adjust wall slice based on player's jump offset
                SDL_RenderDrawLine(renderer, WINDOW_WIDTH - i, yi + player.z + jump_offset, WINDOW_WIDTH - i, yi + height + player.z + jump_offset); // Draw vertical slice of wall
            } else {
                SDL_SetRenderDrawColor(renderer, 255 * color, 255 * color, 255 * color, 255); // Set ray color for debugging
                SDL_RenderDrawLine(renderer, player.x, player.y, smallest_intersection[0], smallest_intersection[1]); // Draw ray from player to intersection
            }
        }

        // Reset smallest intersection values for the next ray
        smallest_intersection[0] = 0;
        smallest_intersection[1] = 0;
        smallest_intersection[2] = INFINITY;
    }

    if(!FIRST_PERSON) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set ray color for debugging
        
        SDL_RenderDrawLine(renderer, player.x + 10*cos(player.angle) - 10*plane_vector[0], player.y+10*sin(player.angle) - 10*plane_vector[1], player.x + 10*cos(player.angle) + 10*plane_vector[0], player.y+10*sin(player.angle) + 10*plane_vector[1]); // Draw ray from player to intersection
    }
}

/* 
    Renders the background including sky and floor.
    Handles the rendering of the floor texture and the sky color.
*/
void render_background(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 48, 73, 255); // Set color to blue for the sky
    SDL_RenderClear(renderer); // Clear the screen with the sky color

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255); // Set color to dark grey for the floor
    if(player.is_jumping) { // If the player is jumping, render a dynamic floor effect
        for(int i = 0; i < WINDOW_WIDTH; i++) { // Loop through each column of the screen
            SDL_RenderDrawLine(renderer, i, FLOOR_SIZE + player.z + 0.7 * player.z * cos(((i - WINDOW_WIDTH/8) * FOV / WINDOW_WIDTH) / 4), i, WINDOW_HEIGHT); // Draw floor line with jump offset
        } 
    } else { // If the player is not jumping, render a solid floor rectangle
        SDL_Rect floor_rect = {
            0, 
            (int)WINDOW_HEIGHT - FLOOR_SIZE, 
            (int)WINDOW_WIDTH, 
            (int)FLOOR_SIZE
        };
        SDL_RenderFillRect(renderer, &floor_rect); // Fill the rectangle with the floor color
    }
}

/* 
    Main rendering function that handles background, map, and camera rendering.
    Parameters: 
        - SDL_Renderer* renderer: the renderer used for drawing
*/
void render(SDL_Renderer* renderer) {
    render_background(renderer); // Render the sky and floor

    if(!FIRST_PERSON) 
        render_map(renderer); // Render the map if not in first-person mode

    render_camera(renderer); // Render the 3D camera view using raycasting

    SDL_RenderPresent(renderer); // Present the rendered frame (swap buffers)
}

/* 
    Destroys the SDL window and renderer, and quits SDL subsystems.
    Parameters: 
        - SDL_Window* window: the window to destroy
        - SDL_Renderer* renderer: the renderer to destroy
*/
void destroy_window(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer); // Destroy the renderer
    SDL_DestroyWindow(window); // Destroy the window
    SDL_Quit(); // Quit SDL subsystems
}

/* 
    Main function of the application.
    Initializes the game, enters the game loop, and cleans up resources.
*/
int main(void) {
    SDL_Window* window; // Pointer to the SDL window
    SDL_Renderer* renderer; // Pointer to the SDL renderer

    // Initialize SDL, create window and renderer
    game_is_running = initialize_window(&window, &renderer);

    setup(); // Initialize game objects (e.g., player)

    while(game_is_running) { // Main game loop
        process_inputs(); // Handle user inputs (keyboard and mouse)
        update(); // Update game state (e.g., player position)
        render(renderer); // Render the current game frame
    }

    destroy_window(window, renderer); // Clean up and exit
}
