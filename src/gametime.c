#include <SDL2/SDL.h>  // Include SDL2 library for managing time and rendering

// Function to calculate time difference since a given timestamp (in milliseconds)
// 'ticks' is the time in milliseconds from SDL_GetTicks() or another SDL time function
// Get the current time in milliseconds with SDL_GetTicks(), subtract 'ticks', and convert to seconds
float get_delta_since(Uint32 ticks) {
    return (SDL_GetTicks() - ticks) / 1000.0f;
}

// Function to calculate the time (in seconds) elapsed since the last frame
double get_delta_time(void) {
    static Uint64 last_frame_time = 0;  // Holds the time of the last frame (static to persist between calls)
    double delta_time = (double) (SDL_GetTicks64() - last_frame_time) / 1000.0f;
    
    // Update last_frame_time to the current time for the next call
    last_frame_time = SDL_GetTicks64();
    
    return delta_time;  // Return the time difference in seconds
}