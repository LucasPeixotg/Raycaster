#ifndef ALGEBRA_H
#define ALGEBRA_H

struct line {
    double x0, y0, xf, yf;
};

struct point {
    double x, y;
};

/**
 * Normalizes a 2D vector to have a unit length (magnitude of 1).
 * This function ensures that the direction of the vector is maintained while adjusting its length.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 */
void normalize_vector2(double vector[2]);

/**
 * Computes the magnitude (length) of a 2D vector.
 * This function calculates how long the vector is, regardless of its direction.
 * 
 * @param vector A 2D vector represented as an array of 2 doubles.
 * @return The magnitude (length) of the vector.
 */
double abs_vector2(double vector[2]);

/**
 * Computes the distance from a point to a plane defined by a 2D vector.
 * This function calculates the shortest distance from the point to the line represented by the vector.
 * 
 * @param plane_vector A 2D vector defining the plane (or line).
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 * @return The perpendicular distance from the point to the plane.
 */
double distance_from_line(double plane_vector[2], double x, double y);

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
int intersection_lines(double angle, double xi, double yi, double line[4], double intersection[2]);

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

#endif