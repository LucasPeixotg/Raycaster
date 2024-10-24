#ifndef GAMETIME_H
#define GAMETIME_H

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
float get_delta_since(Uint32 ticks);

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
double get_delta_time(void);

#endif