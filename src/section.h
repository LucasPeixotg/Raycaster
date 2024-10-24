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
    int door_max;             // Maximum number of doors in the section
    int door_count;           // Current count of doors in the section
    struct door* doors;       // List of doors in the section
    
    int wall_max;             // Maximum number of walls in the section
    int wall_count;           // Current count of walls in the section
    struct line* walls;       // List of walls in the section
};

/**
 * Creates a new section with default values.
 * 
 * @param door_max The maximum number of doors allowed in the section.
 * @param wall_max The maximum number of walls allowed in the section.
 * @return struct section* Pointer to the newly created section, or NULL if allocation fails.
 */
struct section* section_create(int door_max, int wall_max);

/**
 * Adds a door to the specified section.
 * 
 * @param section The section to which the door will be added.
 * @param door The line representing the door's position and dimensions.
 * @param dest Pointer to the next door.
 * @return int 0 if the addition was successful, or 1 if an error occurred.
 */
int section_add_door(struct section* section, struct line door, struct door* dest);

/**
 * Adds a wall to the specified section.
 * 
 * @param section The section to which the wall will be added.
 * @param wall The line representing the wall's position and dimensions.
 * @return int 0 if the addition was successful, or 1 if an error occurred.
 */
int section_add_wall(struct section* section, struct line wall);

/**
 * Renders a section and handles rendering additional sections if needed.
 * This function handles cases where rays intersect with doors,
 * requiring rendering of subsequent sections.
 * 
 * @param section The section to be rendered.
 * @param renderer The SDL renderer used for drawing.
 * @param player The player object used for casting rays based on its position and looking angle.
 */
void section_render(struct section* section, SDL_Renderer* renderer, struct player player);

/**
 * Checks for collision between the player and the section's walls.
 * Returns the point of collision if any, or the desired point if no collision occurs.
 * 
 * @param section The section to check for collisions.
 * @param player The player object.
 * @param desired_point The point the player is trying to reach.
 * @return struct point The actual point of collision, or the desired point if no collision occurs.
 */
struct point section_check_collision(struct section* section, struct player* player, struct point desired_point);

/**
 * Determines if the player is attempting to leave the current section through a door.
 * If a door is found, the function returns the section that the player is entering.
 * 
 * @param section The current section.
 * @param player The player object.
 * @param desired_point The point the player is trying to reach.
 * @return struct section* The section the player is entering through the door, or NULL if no door is found.
 */
struct section* section_check_leaving(struct section* section, struct player* player, struct point desired_point);

/**
 * Destroys a section and frees allocated resources.
 * 
 * @param s The section to be destroyed.
 */
void section_destroy(struct section* s);

#endif
