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
    Uint8 r = 144;
    Uint8 g = 144;
    Uint8 b = 144;

    double intersection[2]; // Array to store intersection points
    double angle_off =  FOV / RAYS_NUMBER; // Calculate angle step for each ray

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set draw color for rays
    double smallest_intersection[4] = { 0 , 0 , INFINITY, player.angle}; // Track closest intersection
    double angle, height, distance;
    struct vec2 line_vector = {
        cos(player.angle + PI/2), // Vector perpendicular to player's view direction
        sin(player.angle + PI/2)
    };

    // Cast rays to detect walls
    struct vec2 point;
    for(int i = 0; i < RAYS_NUMBER; i++) {
        for(int j = 0; j < section->wall_max; j++) { // Loop through all walls in the map
            angle = player.angle + (FOV/2) - (angle_off * i); // Calculate ray angle
            normalize_angle(&angle); // Ensure angle is within 0 to 2*PI
            if(intersection_line_ray(angle, player.position.x, player.position.y, section->walls[j], intersection)) { // Check if ray hits a wall
                
                point.x = player.position.x - intersection[0]; 
                point.y = player.position.y - intersection[1];
                
                distance = distance_line_point(line_vector, point); // Calculate perpendicular distance to wall
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
                int yi = WINDOW_HEIGHT - FLOOR_SIZE - height / 2 + player.z_offset * height / 100;
                SDL_RenderDrawLine(renderer, WINDOW_WIDTH - i, yi, WINDOW_WIDTH - i, yi + height); // Draw vertical slice of wall
            } else {
                SDL_SetRenderDrawColor(renderer, 255 * color, 255 * color, 255 * color, 255); // Set ray color for debugging
                SDL_RenderDrawLine(renderer, player.position.x, player.position.y, smallest_intersection[0], smallest_intersection[1]); // Draw ray from player to intersection
            }
        }

        // Reset smallest intersection values for the next ray
        smallest_intersection[0] = 0;
        smallest_intersection[1] = 0;
        smallest_intersection[2] = INFINITY;
    }

    if(!FIRST_PERSON) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set ray color for debugging
        SDL_RenderDrawLine(
            renderer, 
            player.position.x + 10*cos(player.angle) - 10*line_vector.x, 
            player.position.y+10*sin(player.angle) - 10*line_vector.y, 
            player.position.x + 10*cos(player.angle) + 10*line_vector.x, 
            player.position.y+10*sin(player.angle) + 10*line_vector.y
        ); // Draw ray from player to intersection
    }
}

/**
 * Checks for collision between the player and the section's walls.
 * Also checks if the player is attempting to leave the current section through a door.
 * If a door is found, the function returns the section that the player is entering.
 * The desired vec2 is updated to be the real destination
 * 
 * @param section The current section.
 * @param previous_position The vec2 where the player is.
 * @param desired_position The vec2 the player is trying to reach.
 * @return struct section* The section the player is at after walking.
 */
struct section* section_update(struct section* section, struct vec2 previous_position, struct vec2* desired_position, double delta_time) {
    struct line move;
    move.x0 = previous_position.x;
    move.y0 = previous_position.y;
    move.xf = desired_position->x;
    move.yf = desired_position->y;

    struct vec2 move_dir;
    move_dir.x = move.xf - move.x0;
    move_dir.y = move.yf - move.y0;
    move_dir = normalize_vector2(move_dir);


    move.xf += move_dir.x * PLAYER_WIDTH;
    move.yf += move_dir.y * PLAYER_WIDTH;

    struct vec2 intersection;
    struct vec2 normal, p1, p2;
    for(int i = 0; i < section->wall_count; i++) {
        // equations from Jeffrey Thompson collision detection book
        // https://www.jeffreythompson.org/collision-detection/line-line.php

        if(intersection_line_line(move, section->walls[i], &intersection)) {
            intersection = project_point_on_line(section->walls[i], intersection);
            
            normal.x = section->walls[i].yf - section->walls[i].y0;
            normal.y = section->walls[i].x0 - section->walls[i].xf;
            normal = normalize_vector2(normal);

            p1.x = intersection.x - normal.x * PLAYER_WIDTH;
            p1.y = intersection.y - normal.y * PLAYER_WIDTH;
            p2.x = intersection.x + normal.x * PLAYER_WIDTH;
            p2.y = intersection.y + normal.y * PLAYER_WIDTH;

            if(distance_point_point(previous_position, p1) < distance_point_point(previous_position, p2)) {
                desired_position->x = p1.x;
                desired_position->y = p1.y;        
            } else {
                desired_position->x = p2.x;
                desired_position->y = p2.y;
            }
            move.xf = desired_position->x;
            move.yf = desired_position->y;
        }
    }

    move_dir.x = desired_position->x - previous_position.x;
    move_dir.y = desired_position->y - previous_position.y;
    move_dir = normalize_vector2(move_dir);
    
    desired_position->x = previous_position.x + move_dir.x * PLAYER_MOVE_SPEED * delta_time;
    desired_position->y = previous_position.y + move_dir.y * PLAYER_MOVE_SPEED * delta_time;

    return section;
}