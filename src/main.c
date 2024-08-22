// std
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "player.h"
#include "utils.h"
#include "gametime.h"

// MAP
const int map_lines = 19;
float map[100][7] = {
    {0, 0, WINDOW_WIDTH+1, 0, 255, 0, 255},
    {WINDOW_WIDTH+1, 0, WINDOW_WIDTH+1, WINDOW_HEIGHT, 255, 0, 255},
    {WINDOW_WIDTH+1, WINDOW_HEIGHT, 0, WINDOW_HEIGHT+1, 255, 0, 255},
    {0, WINDOW_HEIGHT+1, 0, 0, 255, 0, 255},

    {000, 200, 100, 201, 255, 255, 255},
    {110, 202, 200, 203, 255, 255, 255},
    {210, 204, 300, 205, 255, 255, 255},
    {310, 206, 400, 207, 255, 255, 255},
    {410, 208, 500, 209, 255, 255, 255},

    {000 + 400, 500, 100 + 400, 501, 255, 220, 0},
    {110 + 400, 502, 200 + 400, 503, 255, 220, 0},
    {210 + 400, 504, 300 + 400, 505, 255, 220, 0},
    {310 + 400, 506, 400 + 400, 507, 255, 220, 0},
    {410 + 400, 508, 500 + 400, 509, 255, 220, 0},

    {000 + 200, 400, 100 + 200, 401, 000, 255, 0},
    {110 + 200, 402, 200 + 200, 403, 000, 255, 0},
    {210 + 200, 404, 300 + 200, 405, 000, 255, 0},
    {310 + 200, 406, 400 + 200, 407, 000, 255, 0},
    {410 + 200, 408, 500 + 200, 409, 000, 255, 0},
};


// outside variables
extern Player player;

// global variables
int game_is_running = FALSE;


/*
    Initializes EVERYTHIG from SDL and creates a window

    returns: 0 if could not initialize something,
             1 if it was successful
*/
int initialize_window(SDL_Window** window, SDL_Renderer** renderer) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

    *window = SDL_CreateWindow(
        NULL, 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS 
    );

    if(!*window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return FALSE;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if(!*renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return FALSE;
    }
    
    if(FIRST_PERSON) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
    }

    return TRUE;
}

void setup(void) {
    setup_player();
}

void process_inputs() {
    SDL_Event event;
    SDL_PollEvent(&event);
    
    static int mouse_x=0, mouse_y=0;
    
    if(!FIRST_PERSON) {
        SDL_GetMouseState(&mouse_x, &mouse_y);
        rotate_player_towards(mouse_x, mouse_y);
    } 

    switch(event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE) game_is_running = FALSE;

            if(event.key.keysym.sym == SDLK_w) player.move_set.front = TRUE;
            if(event.key.keysym.sym == SDLK_d) player.move_set.right = TRUE;
            if(event.key.keysym.sym == SDLK_s) player.move_set.back = TRUE;
            if(event.key.keysym.sym == SDLK_a) player.move_set.left = TRUE;
            if(event.key.keysym.sym == SDLK_SPACE) player.move_set.jump = TRUE;
            if(event.key.keysym.sym == SDLK_r) setup_player();

            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_w) player.move_set.front = FALSE;
            if(event.key.keysym.sym == SDLK_d) player.move_set.right = FALSE;
            if(event.key.keysym.sym == SDLK_s) player.move_set.back = FALSE;
            if(event.key.keysym.sym == SDLK_a) player.move_set.left = FALSE;
            if(event.key.keysym.sym == SDLK_SPACE) player.move_set.jump = FALSE;
            break;

        case SDL_MOUSEMOTION:
            if(FIRST_PERSON) {
                SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
                player.rotation = -mouse_x * MOUSE_SENSITIVITY;
            }
            break;
        default:
            player.rotation = 0;

    }
}

void update() {
    double delta = get_delta_time();
    //printf("ANGLE %lf\n", player.angle / 2 / PI * 360);

    update_player(delta);
}

void render_map(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 0; i < map_lines; i++) {
        SDL_RenderDrawLine(renderer, map[i][0], map[i][1], map[i][2], map[i][3]);
    }
}

void render_camera(SDL_Renderer* renderer) {
    /*
        TODO:
            - improve performance
            - clean code
    */
    float intersection[2];
    float angle_off =  FOV / RAYS_NUMBER;

    SDL_SetRenderDrawColor(renderer, 255, 255,255,255);
    float smallest_intersection[4] = { 0 , 0 , INFINITY, player.angle}; 
    float angle, height, distance;
    float plane_vector[2] = {
        cos(player.angle + PI/2),
        sin(player.angle + PI/2)
    };
    for(int i = 0; i < RAYS_NUMBER; i++) {
        int wall_index = 0;
        for(int j = 0; j < map_lines; j++) {
            angle = player.angle + (FOV/2) - (angle_off * i);
            normalize_angle(&angle);
            if(intersection_lines(angle, player.x, player.y, map[j], intersection)) {
                distance = distance_from_plane(plane_vector, player.x -  intersection[0], player.y - intersection[1]); 
                if(distance < smallest_intersection[2]) {
                    smallest_intersection[0] = intersection[0];
                    smallest_intersection[1] = intersection[1];
                    smallest_intersection[2] = distance;
                    smallest_intersection[3] = angle;
                    wall_index = j;
                }
            }
        }

        if(smallest_intersection[2] != INFINITY) {
            float color = smallest_intersection[2] > 600? 0.01 : (1 - smallest_intersection[2] /600);

            if(FIRST_PERSON) {
                SDL_SetRenderDrawColor(renderer, map[wall_index][4]*color, map[wall_index][5]*color, map[wall_index][6]*color, 255);
                height = WINDOW_HEIGHT / (smallest_intersection[2] / WALL_SIZE);
                
                int yi = WINDOW_HEIGHT - FLOOR_SIZE - height/2;
                float jump_offset = + 0.7 * player.z*cos(((i - WINDOW_WIDTH/8) * FOV/WINDOW_WIDTH)/4);
                SDL_RenderDrawLine(renderer, WINDOW_WIDTH - i, yi + player.z + jump_offset, WINDOW_WIDTH - i, yi + height + player.z + jump_offset);
            } else {
                SDL_SetRenderDrawColor(renderer, 255*color, 255*color, 255*color, 255);
                SDL_RenderDrawLine(renderer, player.x, player.y, smallest_intersection[0], smallest_intersection[1]);
            }
        }

        smallest_intersection[0] = 0;
        smallest_intersection[1] = 0;
        smallest_intersection[2] = INFINITY;
    }
}

void render_background(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 48, 73, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    if(player.is_jumping) {
        for(int i = 0; i < WINDOW_WIDTH; i++) {
            SDL_RenderDrawLine(renderer, i, FLOOR_SIZE + player.z + 0.7 * player.z*cos(((i - WINDOW_WIDTH/8) * FOV/WINDOW_WIDTH)/4), i, WINDOW_HEIGHT);
        } 
    } else {
        SDL_Rect floor_rect = {
            0, 
            (int) WINDOW_HEIGHT - FLOOR_SIZE, 
            (int) WINDOW_WIDTH, 
            (int) FLOOR_SIZE
        };
        SDL_RenderFillRect(renderer, &floor_rect);
    }
}

void render(SDL_Renderer* renderer) {
    render_background(renderer);

    if(!FIRST_PERSON)
        render_map(renderer);
    render_camera(renderer);
    //render_player(renderer);

    // buffer swap
    SDL_RenderPresent(renderer);
}

void destroy_window(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void) {
    SDL_Window* window;
    SDL_Renderer* renderer;

    game_is_running = initialize_window(&window, &renderer);

    setup();
    while(game_is_running) {
        process_inputs();
        update();
        render(renderer);
    }

    destroy_window(window, renderer);
}