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

void normalize_angle(float* angle) {
    if(*angle < -PI) {
        *angle += 2 * PI;
    } else if(*angle > PI) {
        *angle -= 2 * PI;
    }
}

int intersection_lines(float angle, float xi, float yi, float line[4], float intersection[2]) {
    static float ao;
    ao = tan(angle);
    static float bo = -1;
    static float co;
    co = yi - xi * tan(angle);

    static float at;
    at = line[1] - line[3];
    static float bt;
    bt = line[2] - line[0];
    static float ct;
    ct = line[3] * line[0] - line[1] * line[2];
 
    if(ao * bt - at * bo == 0) return 0;

    intersection[0] = (bo * ct - bt * co) / (ao * bt - at * bo);
    intersection[1] = (at * co - ao * ct) / (ao * bt - at * bo);

    float maxy = line[1] > line[3] ? line[1] : line[3];
    float miny = line[1] < line[3] ? line[1] : line[3];

    float maxx = line[0] > line[2] ? line[0] : line[2];
    float minx = line[0] < line[2] ? line[0] : line[2];

    if(intersection[1] > maxy || intersection[1] < miny || intersection[0] < minx || intersection[0] > maxx) {
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

float distance_from_plane(float plane_vector[2], float x, float y) {
    float proj = (plane_vector[0]*x + plane_vector[1]*y)/(plane_vector[0]*plane_vector[0] + plane_vector[1]*plane_vector[1]);
    float orto_proj[2] = {
        x - proj * plane_vector[0],
        y - proj * plane_vector[1]
    };
    return abs_vector2(orto_proj);
}