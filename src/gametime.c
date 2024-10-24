#include <SDL2/SDL.h>  // Include SDL2 library for managing time and rendering

/**
 * Calculates the time difference since a given timestamp (in milliseconds).
 * 
 * This function computes the time passed since the provided timestamp, `ticks`, 
 * which is the time in milliseconds obtained from SDL_GetTicks(). 
 * The result is returned in seconds for ease of use in time-based calculations.
 * 
 * @param ticks The starting time in milliseconds (typically from SDL_GetTicks()).
 * @return float The time difference in seconds.
 */
float get_delta_since(Uint32 ticks) {
    return (SDL_GetTicks() - ticks) / 1000.0f;  // Convert milliseconds to seconds
}

/**
 * Calculates the time (in seconds) elapsed since the last frame.
 * 
 * This function computes the delta time between frames, which is useful for 
 * rendering and updating animations or movement based on frame time.
 * 
 * It uses a static variable to store the timestamp of the previous frame 
 * and updates this value on every call to the current time.
 * 
 * @return double The time elapsed since the last frame, in seconds.
 */
double get_delta_time(void) {
    static Uint64 last_frame_time = 0;  // Holds the time of the last frame
    double delta_time = (double)(SDL_GetTicks64() - last_frame_time) / 1000.0f;
    
    // Update last_frame_time to the current time for the next call
    last_frame_time = SDL_GetTicks64();
    
    return delta_time;  // Return the time difference in seconds
}
