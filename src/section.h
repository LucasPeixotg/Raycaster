#ifndef SECTION_H
#define SECTION_H

#include <SDL2/SDL.h> // SDL library for graphics
#include "algebra.h"
#include "player.h"

// Structure representing a door in the section
struct door {
    struct line position;      // Position and dimensions of the door
    struct door* dest;         // Pointer to the next door
    struct section* room;      // Pointer to the section (room) this door belongs to
};

// Structure representing a section (room)
struct section {
    int door_max;
    int door_count;
    struct door* doors;                        // List of doors in the section
    
    int wall_max;
    int wall_count;
    struct line* walls;        // List of walls in the section
};

// Creates a new section with default values
struct section* section_create(int door_max, int wall_max);

// return error or not
int section_add_door(struct section* section, struct line door, struct door* dest);

// return error or not
void section_add_wall(struct section* section, struct line wall);


/*
    Renders a section and handles rendering additional sections if needed.
    This function handles cases where rays intersect with doors,
    requiring rendering of subsequent sections.
    Parameters:
        - struct section* s: The section to be rendered
        - SDL_Renderer* renderer: The SDL renderer used for drawing
        - struct player*: The player object (used for casting the rays based on its position and looking angle)
*/
void section_render(struct section* section, SDL_Renderer* renderer, struct player* player);

/*
    Checks for collision between the player and the section's walls.
    Returns the point of collision if any, or the desired point if no collision occurs.
    Parameters:
        - struct section* section: The section to check for collisions
        - struct player*: The player object
        - struct point desired_point: The point the player is trying to reach
    Returns:
        - struct point: The actual point of collision, or the desired point if no collision
*/
struct point section_check_collision(struct section* section, struct player* player, struct point desired_point);

/*
    Determines if the player is attempting to leave the current section through a door.
    If a door is found, the function returns the section that the player is entering.
    Parameters:
        - struct section* section: The current section
        - struct player: The player object
        - struct point desired_point: The point the player is trying to reach
    Returns:
        - struct section*: The section the player is entering through the door, or NULL if no door is found
*/
struct section* section_check_leaving(struct section* section, struct player* player, struct point desired_point);

/* 
    Destroys a section and frees allocated resources.
    Parameters:
        - struct section*: The section to be destroyed
*/
void section_destroy(struct section* s);


#endif
