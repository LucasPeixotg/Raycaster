#include "section.h"

#include <malloc.h>

/**
 * Creates a new section with default values.
 * 
 * @param door_max The maximum number of doors allowed in the section.
 * @param wall_max The maximum number of walls allowed in the section.
 * @return struct section* Pointer to the newly created section, or NULL if allocation fails.
 */
struct section* section_create(int door_max, int wall_max) {
    struct section* new = malloc(sizeof(struct section));
    if(new == NULL) return NULL; 

    new->doors = malloc(sizeof(struct door) * door_max);
    if(new->doors == NULL) {
        free(new);
        return NULL;
    }

    new->walls = malloc(sizeof(struct line) * wall_max);
    if(new->walls == NULL) {
        free(new->doors);
        free(new);
        return NULL;
    }

    new->wall_count = 0;
    new->wall_max = wall_max;
    new->door_count = 0;
    new->door_max = door_max;

    return new;
}

/**
 * Adds a door to the specified section.
 * 
 * @param section The section to which the door will be added.
 * @param door The line representing the door's position and dimensions.
 * @param dest Pointer to the next door.
 * @return int 0 if the addition was successful, or 1 if an error occurred.
 */
int section_add_door(struct section* section, struct line door, struct door* dest) {
    if(section->door_count == section->door_max) return 1;
    section->doors[section->door_count].dest = dest;
    section->doors[section->door_count].position = door;
    section->door_count++;
    return 0; 
}

/**
 * Adds a wall to the specified section.
 * 
 * @param section The section to which the wall will be added.
 * @param wall The line representing the wall's position and dimensions.
 * @return int 0 if the addition was successful, or 1 if an error occurred.
 */
int section_add_wall(struct section* section, struct line wall) {
    if(section->wall_count == section->wall_max) return 1;
    section->walls[section->wall_count] = wall;
    section->wall_count++;
    return 0;
}
