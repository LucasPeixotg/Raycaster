// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> // SDL library for graphics

#include "constants.h" // Project constants (screen size, player attributes, etc.)
#include "player.h"    // Player structure and functions
#include "algebra.h"     // Utility functions for the game
#include "gametime.h"  // Time handling functions
#include "section.h"
#include "levels.h"


// External variables defined in player.h
extern struct player player;

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
void render_map(SDL_Renderer* renderer, struct section* section) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white for lines
    for(int i = 0; i < section->wall_count; i++) { // Loop through all lines in the map
        SDL_RenderDrawLine(renderer, section->walls[i].x0, section->walls[i].y0, section->walls[i].xf, section->walls[i].yf); // Draw each line
    }
}

/* 
    Renders the camera (3D view) using raycasting.
    Casts rays from the player's viewpoint, calculates intersections with walls, 
    and renders vertical slices representing walls.
*/
void render_camera(SDL_Renderer* renderer, struct section* section) {
    section_render(section, renderer, player);
}

/* 
    Renders the background including sky and floor.
    Handles the rendering of the floor texture and the sky color.
*/
void render_background(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 150, 150, 180, 55); // Set color for the sky
    SDL_RenderClear(renderer); // Clear the screen with the sky color

    SDL_SetRenderDrawColor(renderer, 0, 0, 10, 255); // Set color for the floor
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
void render(SDL_Renderer* renderer, struct section* section) {
    render_background(renderer); // Render the sky and floor

    render_camera(renderer, section); // Render the 3D camera view using raycasting

    if(!FIRST_PERSON) 
        render_map(renderer, section); // Render the map if not in first-person mode
    
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

    struct section* section = create_level_1();

    while(game_is_running) { // Main game loop
        process_inputs(); // Handle user inputs (keyboard and mouse)
        update(); // Update game state (e.g., player position)
        render(renderer, section); // Render the current game frame
    }

    destroy_window(window, renderer); // Clean up and exit
}
