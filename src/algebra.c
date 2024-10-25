#include <math.h>
#include <stdio.h>
#include "constants.h"
#include "algebra.h"

/**
 * Normalizes a 2D vector to have a unit length (magnitude of 1).
 * This function ensures that the direction of the vector is maintained while adjusting its length.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 */
struct vec2 normalize_vector2(struct vec2 vector) {
    struct vec2 new;
    double len = sqrt(vector.x*vector.x + vector.y*vector.y); // Calculate vector length
    new.x = vector.x / (len ? len : 1); // Avoid division by zero
    new.y = vector.y / (len ? len : 1); // Normalize each component
    return new;
}

/**
 * Computes the magnitude (length) of a 2D vector.
 * This function calculates how long the vector is, regardless of its direction.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 * @return The magnitude (length) of the vector.
 */
double abs_vector2(struct vec2 vector) {
    return (double) sqrt(vector.x*vector.x + vector.y*vector.y); // Calculate magnitude
}

/**
 * Computes the distance from a vec2 to a line defined by a 2D vector.
 * This function calculates the shortest distance from the vec2 to the line represented by the vector.
 * 
 * @param plane_vector A 2D vector defining the line.
 * @param x The x-coordinate of the vec2.
 * @param y The y-coordinate of the vec2.
 * @return The perpendicular distance from the vec2 to the plane.
 */
double distance_line_point(struct vec2 line_vector, struct vec2 point) {
    double proj = (line_vector.x*point.x + line_vector.y*point.y) / (line_vector.x*line_vector.x + line_vector.y*line_vector.y);
    struct vec2 orto_proj = {
        point.x - proj * line_vector.x, // Orthogonal projection of the vec2
        point.y - proj * line_vector.y
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
 * Determines the intersection vec2 of a ray with a line segment.
 * This function calculates where a ray, defined by an angle and starting vec2, intersects a line segment,
 * if at all, and checks if the intersection is within the bounds of the segment.
 * 
 * @param angle The angle of the ray (in radians).
 * @param xi The starting x-coordinate of the ray.
 * @param yi The starting y-coordinate of the ray.
 * @param line An array representing the coordinates of the line segment (x1, y1, x2, y2).
 * @param intersection An array to store the coordinates of the intersection vec2 (output).
 * @return 1 if the intersection is valid and within the bounds of the line segment, 0 otherwise.
 */
int intersection_line_ray(double angle, double xi, double yi, struct line line, double intersection[2]) {
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

    at = line.y0 - line.yf; // Slope of the line segment
    bt = line.xf - line.x0; // X-component of the line segment direction
    ct = line.yf * line.x0 - line.y0 * line.xf; // Y-intercept of the line segment
    
    // Check if the lines are parallel
    if(ao * bt - at * bo == 0) return 0;

    // check if the line is horizontal
    if(at == 0) {
        // Check if the horizontal line is parallel to the Y-axis (same height)
        if (yi == line.y0) return 0; // The ray is parallel and does not intersect with the line

        // Calculate the intersection
        intersection[1] = line.y0; // The intersection occurs at the height of the line
        intersection[0] = xi + (intersection[1] - yi) / ao; // Calculate the X coordinate of the intersection

        // Check if the intersection is within the bounds of the line
        if (intersection[0] < fmin(line.x0, line.xf) || intersection[0] > fmax(line.x0, line.xf))
            return 0; 

        // Check if the intersection is behind player's eyes
        if((angle < 0 && line.y0 > yi) || (angle > 0 && line.y0 < yi)) 
            return 0;

        return 1;
    }

    // Calculate intersection vec2
    intersection[0] = (bo * ct - bt * co) / (ao * bt - at * bo);
    intersection[1] = (at * co - ao * ct) / (ao * bt - at * bo);

    // Check if the intersection vec2 is within the line segment bounds
    double maxy = line.y0 > line.yf ? line.y0 : line.yf;
    double miny = line.y0 < line.yf ? line.y0 : line.yf;
    double maxx = line.x0 > line.xf ? line.x0 : line.xf;
    double minx = line.x0 < line.xf ? line.x0 : line.xf;

    if(intersection[1] > maxy || intersection[1] < miny || intersection[0] < minx || intersection[0] > maxx) {
        return 0; // Intersection is outside the line segment
    }
    
    // Determine if the intersection vec2 is in front of the ray origin based on the angle
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

int intersection_line_line(struct line a, struct line b, struct vec2* dest) {
    float x1 = a.x0;
    float y1 = a.y0;
    float x2 = a.xf;
    float y2 = a.yf;
    float x3 = b.x0;
    float y3 = b.y0;
    float x4 = b.xf;
    float y4 = b.yf;
    
    float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        dest->x = x1 + (uA * (x2-x1));
        dest->y = y1 + (uA * (y2-y1));

        return 1;
    }
    return 0;
}

struct vec2 project_point_on_line(struct line line, struct vec2 point) {
    // equations found on StackOverflow 
    // https://stackoverflow.com/questions/64330618/finding-the-projection-of-a-point-onto-a-line
    struct vec2 AB;
    AB.x = line.xf - line.x0;
    AB.y = line.yf - line.y0;

    struct vec2 AC;
    AC.x = point.x - line.x0;
    AC.y = point.y - line.y0;

    float coeff = (AB.x*AC.x + AB.y*AC.y) / (AB.x*AB.x+AB.y*AB.y);
    struct vec2 projection;
    projection.x = line.x0 + AB.x * coeff;
    projection.y = line.y0 + AB.y * coeff;
    
    return projection;
}

double distance_point_point(struct vec2 a, struct vec2 b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}