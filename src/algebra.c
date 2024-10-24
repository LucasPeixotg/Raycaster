#include <math.h>
#include <stdio.h>
#include "constants.h"

/**
 * Normalizes a 2D vector to have a unit length (magnitude of 1).
 * This function ensures that the direction of the vector is maintained while adjusting its length.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 */
void normalize_vector2(double vector2[2]) {
    double len = sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]); // Calculate vector length
    vector2[0] = vector2[0] / (len ? len : 1); // Avoid division by zero
    vector2[1] = vector2[1] / (len ? len : 1); // Normalize each component
}

/**
 * Computes the magnitude (length) of a 2D vector.
 * This function calculates how long the vector is, regardless of its direction.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 * @return The magnitude (length) of the vector.
 */
double abs_vector2(double vector2[2]) {
    return (double) sqrt(vector2[0]*vector2[0] + vector2[1]*vector2[1]); // Calculate magnitude
}

/**
 * Computes the distance from a point to a line defined by a 2D vector.
 * This function calculates the shortest distance from the point to the line represented by the vector.
 * 
 * @param plane_vector A 2D vector defining the line.
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return The perpendicular distance from the point to the plane.
 */
double distance_from_line(double plane_vector[2], double x, double y) {
    double proj = (plane_vector[0]*x + plane_vector[1]*y) / (plane_vector[0]*plane_vector[0] + plane_vector[1]*plane_vector[1]);
    double orto_proj[2] = {
        x - proj * plane_vector[0], // Orthogonal projection of the point
        y - proj * plane_vector[1]
    };
    return abs_vector2(orto_proj); // Return the magnitude of the orthogonal projection
}

/**
 * Normalizes an angle to fall within the range [-PI, PI].
 * This function ensures that the angle is adjusted to stay within the standard circular range.
 * 
 * @param angle A pointer to the angle to be normalized (in radians).
 */
void normalize_angle(double* angle) {
    if(*angle < -PI) {
        *angle += 2 * PI; // Adjust angle if it's less than -PI
    } else if(*angle > PI) {
        *angle -= 2 * PI; // Adjust angle if it's greater than PI
    }
}

/**
 * Determines the intersection point of a ray with a line segment.
 * This function calculates where a ray, defined by an angle and starting point, intersects a line segment,
 * if at all, and checks if the intersection is within the bounds of the segment.
 * 
 * @param angle The angle of the ray (in radians).
 * @param xi The starting x-coordinate of the ray.
 * @param yi The starting y-coordinate of the ray.
 * @param line An array representing the coordinates of the line segment (x1, y1, x2, y2).
 * @param intersection An array to store the coordinates of the intersection point (output).
 * @return 1 if the intersection is valid and within the bounds of the line segment, 0 otherwise.
 */
int intersection_lines(double angle, double xi, double yi, double line[4], double intersection[2]) {
    // a lot of variables are declared as static to avoid a lot of allocations,
    // because this function is one of the most called functions in the application
    static double ao;
    static double co;
    static double at;
    static double bt;
    static double ct;

    ao = tan(angle); // Slope of the ray

    static double bo = -1; // Y-intercept of the ray
    co = yi - xi * ao;

    at = line[1] - line[3]; // Slope of the line segment
    bt = line[2] - line[0]; // X-component of the line segment direction
    ct = line[3] * line[0] - line[1] * line[2]; // Y-intercept of the line segment
    
    // Check if the lines are parallel
    if(ao * bt - at * bo == 0) return 0;

    // check if the line is horizontal
    if(at == 0) {
        // Check if the horizontal line is parallel to the Y-axis (same height)
        if (yi == line[1]) return 0; // The ray is parallel and does not intersect with the line

        // Calculate the intersection
        intersection[1] = line[1]; // The intersection occurs at the height of the line
        intersection[0] = xi + (intersection[1] - yi) / ao; // Calculate the X coordinate of the intersection

        // Check if the intersection is within the bounds of the line
        if (intersection[0] < fmin(line[0], line[2]) || intersection[0] > fmax(line[0], line[2]))
            return 0; 

        // Check if the intersection is behind player's eyes
        if((angle < 0 && line[1] > yi) || (angle > 0 && line[1] < yi)) 
            return 0;

        return 1;
    }

    // Calculate intersection point
    intersection[0] = (bo * ct - bt * co) / (ao * bt - at * bo);
    intersection[1] = (at * co - ao * ct) / (ao * bt - at * bo);

    // Check if the intersection point is within the line segment bounds
    double maxy = line[1] > line[3] ? line[1] : line[3];
    double miny = line[1] < line[3] ? line[1] : line[3];
    double maxx = line[0] > line[2] ? line[0] : line[2];
    double minx = line[0] < line[2] ? line[0] : line[2];

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
