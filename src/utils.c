#include <math.h>
#include <stdio.h>
#include "constants.h"

/*
    Normalizes a 2D vector to unit length.
    Ensures that the vector maintains its direction but has a length of 1.
    Parameters:
        - float vector2[2]: The vector to be normalized
*/
void normalize_vector2(float vector2[2]) {
    float len = sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]); // Calculate vector length
    vector2[0] = vector2[0] / (len ? len : 1); // Avoid division by zero
    vector2[1] = vector2[1] / (len ? len : 1); // Normalize each component
}

/*
    Computes the magnitude (length) of a 2D vector.
    Parameters:
        - float vector2[2]: The vector whose magnitude is to be computed
    Returns:
        - float: The magnitude of the vector
*/
float abs_vector2(float vector2[2]) {
    return (float) sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]); // Calculate magnitude
}

/*
    Normalizes an angle to the range [-PI, PI].
    Ensures that the angle stays within the specified bounds.
    Parameters:
        - float* angle: Pointer to the angle to be normalized
*/
void normalize_angle(float* angle) {
    if(*angle < -PI) {
        *angle += 2 * PI; // Adjust angle if it's less than -PI
    } else if(*angle > PI) {
        *angle -= 2 * PI; // Adjust angle if it's greater than PI
    }
}

/*
    Computes the intersection point of a ray and a line segment.
    Checks if the intersection point lies within the bounds of the line segment.
    Parameters:
        - float angle: The angle of the ray
        - float xi, yi: The starting coordinates of the ray
        - float line[4]: Coordinates of the line segment (x1, y1, x2, y2)
        - float intersection[2]: Array to store the intersection coordinates
    Returns:
        - int: 1 if the intersection is valid and within bounds, 0 otherwise
*/
int intersection_lines(float angle, float xi, float yi, float line[4], float intersection[2]) {
    static float ao;
    ao = tan(angle); // Slope of the ray

    static float bo = -1; // Y-intercept of the ray
    static float co;
    co = yi - xi * tan(angle);

    static float at;
    at = line[1] - line[3]; // Slope of the line segment
    static float bt;
    bt = line[2] - line[0]; // X-component of the line segment direction
    static float ct;
    ct = line[3] * line[0] - line[1] * line[2]; // Y-intercept of the line segment
    
    // Check if the lines are parallel
    if(ao * bt - at * bo == 0) return 0;

    // Calculate intersection point
    intersection[0] = (bo * ct - bt * co) / (ao * bt - at * bo);
    intersection[1] = (at * co - ao * ct) / (ao * bt - at * bo);

    // Check if the intersection point is within the line segment bounds
    float maxy = line[1] > line[3] ? line[1] : line[3];
    float miny = line[1] < line[3] ? line[1] : line[3];
    float maxx = line[0] > line[2] ? line[0] : line[2];
    float minx = line[0] < line[2] ? line[0] : line[2];

    if(intersection[1] > maxy || intersection[1] < miny || intersection[0] < minx || intersection[0] > maxx) {
        return 0; // Intersection is outside the line segment
    }
    
    // Determine if the intersection point is in front of the ray origin based on the angle
    if(angle < 0) {
        if(angle > -(PI / 2) && xi < intersection[0] && yi > intersection[1]) {
            return 1; // Valid intersection
        } else if(angle < -(PI / 2) && xi > intersection[0] && yi > intersection[1]) {
            return 1; // Valid intersection
        } else {
            return 0; // Invalid intersection
        }
    } else {
        if(angle < (PI / 2) && xi < intersection[0] && yi < intersection[1]) {
            return 1; // Valid intersection
        } else if(angle > (PI / 2) && xi > intersection[0] && yi < intersection[1]) {
            return 1; // Valid intersection
        } else {
            return 0; // Invalid intersection
        }
    }

    return 1; // Default return value (should not be reached)
}

/*
    Computes the distance from a point to a plane defined by a vector.
    Parameters:
        - float plane_vector[2]: The vector defining the plane
        - float x, y: The point coordinates
    Returns:
        - float: The distance from the point to the plane
*/
float distance_from_plane(float plane_vector[2], float x, float y) {
    float proj = (plane_vector[0]*x + plane_vector[1]*y) / (plane_vector[0]*plane_vector[0] + plane_vector[1]*plane_vector[1]);
    float orto_proj[2] = {
        x - proj * plane_vector[0], // Orthogonal projection of the point
        y - proj * plane_vector[1]
    };
    return abs_vector2(orto_proj); // Return the magnitude of the orthogonal projection
}