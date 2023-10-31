// std
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "player.h"
#include "utils.h"

// MAP
const int map_lines = 9;
float map[9][4] = {
    {200, 200, 200, 900},
    {200, 900, 500, 220},
    {500, 220, 000, 310},
    {000, 310, 000, WINDOW_HEIGHT},
    {WINDOW_WIDTH, 200, 000, 310},

    {0, 0, WINDOW_WIDTH, 10},
    {WINDOW_WIDTH, 10, WINDOW_WIDTH, WINDOW_HEIGHT},
    {WINDOW_WIDTH, WINDOW_HEIGHT, 0, WINDOW_HEIGHT+10},
    {0, WINDOW_HEIGHT+10, 0, 0},
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
            if(event.key.keysym.sym == SDLK_r) setup_player();

            break;
        case SDL_KEYUP:
            if(event.key.keysym.sym == SDLK_w) player.move_set.front = FALSE;
            if(event.key.keysym.sym == SDLK_d) player.move_set.right = FALSE;
            if(event.key.keysym.sym == SDLK_s) player.move_set.back = FALSE;
            if(event.key.keysym.sym == SDLK_a) player.move_set.left = FALSE;
            break;

        case SDL_MOUSEMOTION:
            if(FIRST_PERSON) {
                SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
                printf("STATE: (%d, %d)\n", mouse_x, mouse_y);
                player.rotation = -mouse_x * MOUSE_SENSITIVITY;
            }
            break;
        default:
            player.rotation = 0;

    }
}

void update() {
    update_player();
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
    float angle;
    for(int i = 0; i < RAYS_NUMBER; i++) {
        for(int j = 0; j < map_lines; j++) {
            angle = player.angle + (FOV/2) - (angle_off * i);
            normalize_angle(&angle);
            if(intersection_lines(angle, player.x, player.y, map[j], intersection)) {
                if(ne_distance_between_points(player.x, player.y, intersection[0], intersection[1]) < smallest_intersection[2]) {
                    smallest_intersection[0] = intersection[0];
                    smallest_intersection[1] = intersection[1];
                    smallest_intersection[2] = ne_distance_between_points(player.x, player.y, intersection[0], intersection[1]);
                    smallest_intersection[3] = angle;
                }
            }
        }

        if(smallest_intersection[2] != INFINITY) {
            float color = smallest_intersection[2] > 400? 0.01 : (1 - smallest_intersection[2] /400);
            SDL_SetRenderDrawColor(renderer, 141*color, 170*color, 145*color, 255);
            if(FIRST_PERSON) {
                float height = WINDOW_HEIGHT / (smallest_intersection[2] / WALL_SIZE);
                
                //SDL_RenderDrawRect(renderer, &rect);
                int yi = WINDOW_HEIGHT - FLOOR_SIZE - height/2;
                SDL_RenderDrawLine(renderer, WINDOW_WIDTH - i, yi, WINDOW_WIDTH - i, yi + height);
            } else {
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
    static SDL_Rect ceil_rect = {
        0, 
        0, 
        (int) WINDOW_WIDTH, 
        (int) WINDOW_HEIGHT - FLOOR_SIZE
    };
    SDL_RenderFillRect(renderer, &ceil_rect);

    SDL_SetRenderDrawColor(renderer, 50, 2, 31, 255);
    static SDL_Rect floor_rect = {
        0, 
        (int) WINDOW_HEIGHT - FLOOR_SIZE, 
        (int) WINDOW_WIDTH, 
        (int) FLOOR_SIZE
    };
    SDL_RenderFillRect(renderer, &floor_rect);

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