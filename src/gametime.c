#include <SDL2/SDL.h>

float get_delta_since(Uint32 ticks) {
    return (SDL_GetTicks() - ticks) / 1000.0f;
}

double get_delta_time(void) {
    static Uint64 last_frame_time = 0;
    double delta_time = (double) (SDL_GetTicks64() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks64();
    return delta_time;
}
