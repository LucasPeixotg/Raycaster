#ifndef ALGEBRA_H
#define ALGEBRA_H

struct line {
    double x0;
    double y0;
    double xf;
    double yf;
};

/*
    Normalizes a 2D vector to have a unit length (magnitude of 1).
    This function ensures that the direction of the vector is maintained while adjusting its length.
    Parameters:
        - double vector[2]: The 2D vector to be normalized
*/
void normalize_vector2(double vector[2]);

/*
    Computes the magnitude (length) of a 2D vector.
    This function calculates how long the vector is, regardless of its direction.
    Parameters:
        - double vector[2]: The 2D vector whose magnitude is to be computed
    Returns:
        - double: The magnitude of the vector
*/
double abs_vector2(double vector[2]);

/*
    Computes the distance from a point to a plane defined by a 2D vector.
    This function calculates the shortest distance from the point to the line represented by the vector.
    Parameters:
        - double plane_vector[2]: The vector that defines the plane or line
        - double x, y: The coordinates of the point
    Returns:
        - double: The perpendicular distance from the point to the plane
*/
double distance_from_plane(double plane_vector[2], double x, double y);

/*
    Determines the intersection point of a ray with a line segment.
    This function calculates where a ray, defined by an angle and starting point, intersects a line segment, 
    if at all, and checks if the intersection is within the bounds of the segment.
    Parameters:
        - double angle: The angle of the ray (in radians)
        - double xi, yi: The starting coordinates of the ray
        - double line[4]: Coordinates of the line segment (x1, y1, x2, y2)
        - double intersection[2]: Array to store the coordinates of the intersection point
    Returns:
        - int: 1 if the intersection is valid and within the line segment bounds, 0 otherwise
*/
int intersection_lines(double angle, double xi, double yi, double line[4], double intersection[2]);

/*
    Normalizes an angle to fall within the range [-PI, PI].
    This function ensures that the angle is adjusted to stay within the standard circular range.
    Parameters:
        - double* angle: Pointer to the angle to be normalized
*/
void normalize_angle(double* angle);

/*
    Projects one 2D vector onto another 2D vector.
    This function calculates the projection of vector `u` onto vector `v`.
    Parameters:
        - double u[2]: The vector to be projected
        - double v[2]: The vector onto which `u` is projected
    Returns:
        - double: The magnitude of the projection of `u` onto `v`
*/
double project_vector2(double u[2], double v[2]);

#endif