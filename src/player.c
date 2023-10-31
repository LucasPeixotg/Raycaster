// std
#include <math.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "gametime.h"
#include "utils.h"
#include "player.h"

Player player;

void setup_player(void) {
    player.width = PLAYER_WIDTH;
    player.height = PLAYER_HEIGHT;

    player.x = WINDOW_WIDTH / 2.0f - player.width / 2.0f;
    player.y = WINDOW_HEIGHT / 2.0f - player.height / 2.0f;
    
    player.velocity[0] = 0;
    player.velocity[1] = 0;
    player.rotation = 0;
    
    player.move_set.front = FALSE;
    player.move_set.back  = FALSE;
    player.move_set.right = FALSE;
    player.move_set.left  = FALSE;
    
    player.possible_moves.front    = TRUE;
    player.possible_moves.back  = TRUE;
    player.possible_moves.right = TRUE;
    player.possible_moves.left  = TRUE;
}


void set_move_player(void) {
    player.velocity[0] = 0;
    player.velocity[1] = 0;

    if(player.move_set.right && player.possible_moves.right) {
        player.velocity[0] += cos(player.angle + PI/2);
        player.velocity[1] += sin(player.angle + PI/2);
    } else if(player.move_set.left && player.possible_moves.left) {
        player.velocity[0] += cos(player.angle - PI/2);
        player.velocity[1] += sin(player.angle - PI/2);
    }

    if(player.move_set.front && player.possible_moves.back) {
        player.velocity[0] += cos(player.angle);
        player.velocity[1] += sin(player.angle);
    } else if(player.move_set.back && player.possible_moves.back) {
        player.velocity[0] += cos(player.angle + PI);
        player.velocity[1] += sin(player.angle + PI);
    }

    normalize_vector2(player.velocity);
    player.velocity[0] *= PLAYER_MOVE_SPEED;
    player.velocity[1] *= PLAYER_MOVE_SPEED;
}

void update_player(void) {
    set_move_player();

    float delta_time = get_delta_time();

    player.x += player.velocity[0] * delta_time;
    player.y += player.velocity[1] * delta_time;

    player.angle -= player.rotation * PLAYER_ROTATION_SPEED * delta_time;
    normalize_angle(&player.angle);
}

void render_player(SDL_Renderer* renderer) {
    SDL_Rect player_rect = {
        (int) (player.x - player.width / 2),
        (int) (player.y- player.height / 2),
        (int) player.width,
        (int) player.height
    };

    SDL_SetRenderDrawColor(renderer, 200, 100, 100, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(
        renderer, 
        (int) player.x, 
        (int) player.y, 
        (int) (player.x + cos(player.angle) * player.width), 
        (int) (player.y + sin(player.angle) * player.width)
    );
}

void rotate_player_towards(int x, int y) {
    player.angle = atan2(y - player.y, x - player.x);
}
