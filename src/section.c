#include "section.h"

#include <malloc.h>

// Creates a new section with default values
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

// return error or not
int section_add_door(struct section* section, struct line door, struct door* dest) {
    if(section->door_count == section->door_max) return 1;
    section->doors[section->door_count].dest = dest;
    section->doors[section->door_count].position = door;
    section->door_count++;
    return 0; 
}

// return error or not
int section_add_wall(struct section* section, struct line wall) {
    if(section->wall_count == section->wall_max) return 1;
    section->walls[section->wall_count] = wall;
    section->wall_count++;
    return 0;
}
