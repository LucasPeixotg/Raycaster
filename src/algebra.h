#ifndef ALGEBRA_H
#define ALGEBRA_H

struct line {
    double x0, y0, xf, yf;
};

struct vec2 {
    double x, y;
};

/**
 * Normalizes a 2D vector to have a unit length (magnitude of 1).
 * This function ensures that the direction of the vector is maintained while adjusting its length.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 */
struct vec2 normalize_vector2(struct vec2 vector);

/**
 * Computes the magnitude (length) of a 2D vector.
 * This function calculates how long the vector is, regardless of its direction.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 * @return The magnitude (length) of the vector.
 */
double abs_vector2(struct vec2 vector);

/**
 * Computes the distance from a vec2 to a plane defined by a 2D vector.
 * This function calculates the shortest distance from the vec2 to the line represented by the vector.
 * 
 * @param plane_vector A 2D vector defining the plane (or line).
 * @param x The x-coordinate of the vec2.
 * @param y The y-coordinate of the vec2.
 * @return The perpendicular distance from the vec2 to the plane.
 */
double distance_line_point(struct vec2 line_vector, struct vec2 point);

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
int intersection_line_ray(double angle, double xi, double yi, struct line line, double intersection[2]);

/**
 * Normalizes an angle to fall within the range [-PI, PI].
 * This function ensures that the angle is adjusted to stay within the standard circular range.
 * 
 * @param angle A pointer to the angle to be normalized (in radians).
 */
void normalize_angle(double* angle);

/**
 * Projects one 2D vector onto another 2D vector.
 * This function calculates the projection of vector `u` onto vector `v`.
 * 
 * @param u The vector to be projected.
 * @param v The vector onto which `u` is projected.
 * @return The magnitude of the projection of `u` onto `v`.
 */
double project_vector2(double u[2], double v[2]);

// returns 1 if intersect  0 if dont
int intersection_line_line(struct line a, struct line b, struct vec2* dest);

struct vec2 project_point_on_line(struct line line, struct vec2 point);

double distance_point_point(struct vec2 a, struct vec2 b);

#endif