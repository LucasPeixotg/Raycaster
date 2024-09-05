#ifndef GAMETIME_H
#define GAMETIME_H

// Calculates the time (in seconds) that has passed since a given SDL tick (in milliseconds)
float get_delta_since(Uint32 ticks);

// Calculates the time (in seconds) elapsed since the last frame, used to maintain smooth frame rendering
double get_delta_time(void);

#endif