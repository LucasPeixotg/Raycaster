#include <SDL2/SDL.h>

float get_delta_since(Uint32 ticks) {
    return (SDL_GetTicks() - ticks) / 1000.0f;
}

float get_delta_time(void) {
    static int last_frame_time = 0;
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();
    return delta_time;
}