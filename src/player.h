#include <SDL2/SDL.h>

typedef struct {
    int front;
    int back;
    int right;
    int left;
} MoveSet;

typedef struct {
    float rotation;
    float x;
    float y;
    float width;
    float height;
    float velocity[2];
    float angle;
    MoveSet move_set;
    MoveSet possible_moves;
} Player;


void update_player();

void render_player(SDL_Renderer* renderer);

void setup_player(void);

void set_move_player();

void rotate_player_towards(int x, int y);