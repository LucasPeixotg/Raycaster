#include <math.h>
#include <stdio.h>
#include "constants.h"

void normalize_vector2(float vector2[2]) {
    float len = sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]);
    vector2[0] = vector2[0] / (len? len : 1);
    vector2[1] = vector2[1] / (len? len : 1);
}

float abs_vector2(float vector2[2]) {
    return (float) sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]);
}

int intersection_lines(float angle, float xi, float yi, float line[4], float intersection[2]) {
    if(angle < -PI) {
        angle += 2 * PI;
    } else if(angle > PI) {
        angle -= 2 * PI;
    }

    float ao = tan(angle);
    float bo = -1;
    float co = yi - xi * tan(angle);

    float at = line[1] - line[3];
    float bt = line[2] - line[0];
    float ct = line[3] * line[0] - line[1] * line[2];

    if(ao * bt - at * bo == 0) return 0;

    intersection[0] = (bo * ct - bt * co) / (ao * bt - at * bo);
    intersection[1] = (at * co - ao * ct) / (ao * bt - at * bo);

    float maxy = line[1] > line[3] ? line[1] : line[3];
    float miny = line[1] < line[3] ? line[1] : line[3];

    float maxx = line[0] > line[2] ? line[0] : line[2];
    float minx = line[0] < line[2] ? line[0] : line[2];

    if(isinf(intersection[0]) || isinf(intersection[1])) {
        printf("INFINITY\n");
        return 0;
    } else if(intersection[1] > maxy || intersection[1] < miny || intersection[0] < minx || intersection[0] > maxx) {
        return 0;
    }
        
    
    if(angle < 0) {
        if(angle > -(PI / 2) && xi < intersection[0] && yi > intersection[1]) {
            return 1;
        } else if(angle < -(PI / 2) && xi > intersection[0] && yi > intersection[1]) {
            return 1;
        } else {
            return 0;
        }
    } else {
        if(angle < (PI / 2) && xi < intersection[0] && yi < intersection[1]) {
            return 1;
        } else if(angle > (PI / 2) && xi > intersection[0] && yi < intersection[1]) {
            return 1;
        }else {
            return 0;
        }
    }
    

    return 1;
}

float distance_between_points(float a, float b, float x, float y) {
    return sqrt(pow(a - x, 2) + pow(b - y, 2));
}