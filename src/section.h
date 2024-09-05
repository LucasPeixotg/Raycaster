#ifndef SECTION_H
#define SECTION_H

#include <SDL2/SDL.h> // SDL library for graphics

#include "algebra.h"

struct door {
    struct line position;
    struct door* next_door;
    struct section* room;
};

struct section {
    struct door* doors;
    struct line* walls;
};

struct section* new_section();

/*
render a section, consecutively rendering other sections if needed
(that is, if the rays hit one or more doors)
*/
void render_section(struct section*, SDL_Renderer* renderer);


struct section* section_leaving();


#endif