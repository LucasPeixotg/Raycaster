#include "section.h"
#include "constants.h"

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

/**
 * Renders a section and handles rendering additional sections if needed.
 * This function handles cases where rays intersect with doors,
 * requiring rendering of subsequent sections.
 * 
 * @param section The section to be rendered.
 * @param renderer The SDL renderer used for drawing.
 * @param player The player object used for casting rays based on its position and looking angle.
 */
void section_render(struct section* section, SDL_Renderer* renderer, struct player player) {
    Uint8 r = 12;
    Uint8 g = 12;
    Uint8 b = 12;

    double intersection[2]; // Array to store intersection points
    double angle_off =  FOV / RAYS_NUMBER; // Calculate angle step for each ray

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set draw color for rays
    double smallest_intersection[4] = { 0 , 0 , INFINITY, player.angle}; // Track closest intersection
    double angle, height, distance;
    double plane_vector[2] = {
        cos(player.angle + PI/2), // Vector perpendicular to player's view direction
        sin(player.angle + PI/2)
    };

    // Cast rays to detect walls
    for(int i = 0; i < RAYS_NUMBER; i++) {
        for(int j = 0; j < section->wall_max; j++) { // Loop through all walls in the map
            angle = player.angle + (FOV/2) - (angle_off * i); // Calculate ray angle
            normalize_angle(&angle); // Ensure angle is within 0 to 2*PI
            if(intersection_lines(angle, player.x, player.y, section->walls[j], intersection)) { // Check if ray hits a wall
                distance = distance_from_line(plane_vector, player.x - intersection[0], player.y - intersection[1]); // Calculate perpendicular distance to wall
                if(distance < smallest_intersection[2]) { // Track the closest intersection
                    smallest_intersection[0] = intersection[0];
                    smallest_intersection[1] = intersection[1];
                    smallest_intersection[2] = distance;
                    smallest_intersection[3] = angle;
                }
            }
        }

        // If an intersection was found, render the wall slice
        if(smallest_intersection[2] != INFINITY) {
            float color = smallest_intersection[2] > 600 ? 0.01 : (1 - smallest_intersection[2] / 600); // Diminish brightness with distance
            
            if(FIRST_PERSON) {
                SDL_SetRenderDrawColor(renderer, r*color, g*color, b*color, 255); // Set wall color
                height = WINDOW_HEIGHT / (smallest_intersection[2] / WALL_SIZE); // Calculate wall height

                // Calculate vertical position of the wall slice
                int yi = WINDOW_HEIGHT - FLOOR_SIZE - height / 2;
                float jump_offset = + 0.7 * player.z * cos(((i - WINDOW_WIDTH/8) * FOV / WINDOW_WIDTH) / 4); // Adjust wall slice based on player's jump offset
                SDL_RenderDrawLine(renderer, WINDOW_WIDTH - i, yi + player.z + jump_offset, WINDOW_WIDTH - i, yi + height + player.z + jump_offset); // Draw vertical slice of wall
            } else {
                SDL_SetRenderDrawColor(renderer, 255 * color, 255 * color, 255 * color, 255); // Set ray color for debugging
                SDL_RenderDrawLine(renderer, player.x, player.y, smallest_intersection[0], smallest_intersection[1]); // Draw ray from player to intersection
            }
        }

        // Reset smallest intersection values for the next ray
        smallest_intersection[0] = 0;
        smallest_intersection[1] = 0;
        smallest_intersection[2] = INFINITY;
    }

    if(!FIRST_PERSON) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set ray color for debugging
        
        SDL_RenderDrawLine(renderer, player.x + 10*cos(player.angle) - 10*plane_vector[0], player.y+10*sin(player.angle) - 10*plane_vector[1], player.x + 10*cos(player.angle) + 10*plane_vector[0], player.y+10*sin(player.angle) + 10*plane_vector[1]); // Draw ray from player to intersection
    }

}

