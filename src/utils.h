#ifndef UTILS_H
#define UTILS_H

/*
    Normalizes a 2D vector to have a unit length (magnitude of 1).
    This function ensures that the direction of the vector is maintained while adjusting its length.
    Parameters:
        - float vector[2]: The 2D vector to be normalized
*/
void normalize_vector2(float vector[2]);

/*
    Computes the magnitude (length) of a 2D vector.
    This function calculates how long the vector is, regardless of its direction.
    Parameters:
        - float vector[2]: The 2D vector whose magnitude is to be computed
    Returns:
        - float: The magnitude of the vector
*/
float abs_vector2(float vector[2]);

/*
    Computes the distance from a point to a plane defined by a 2D vector.
    This function calculates the shortest distance from the point to the line represented by the vector.
    Parameters:
        - float plane_vector[2]: The vector that defines the plane or line
        - float x, y: The coordinates of the point
    Returns:
        - float: The perpendicular distance from the point to the plane
*/
float distance_from_plane(float plane_vector[2], float x, float y);

/*
    Determines the intersection point of a ray with a line segment.
    This function calculates where a ray, defined by an angle and starting point, intersects a line segment, 
    if at all, and checks if the intersection is within the bounds of the segment.
    Parameters:
        - float angle: The angle of the ray (in radians)
        - float xi, yi: The starting coordinates of the ray
        - float line[4]: Coordinates of the line segment (x1, y1, x2, y2)
        - float intersection[2]: Array to store the coordinates of the intersection point
    Returns:
        - int: 1 if the intersection is valid and within the line segment bounds, 0 otherwise
*/
int intersection_lines(float angle, float xi, float yi, float line[4], float intersection[2]);

/*
    Normalizes an angle to fall within the range [-PI, PI].
    This function ensures that the angle is adjusted to stay within the standard circular range.
    Parameters:
        - float* angle: Pointer to the angle to be normalized
*/
void normalize_angle(float* angle);

/*
    Projects one 2D vector onto another 2D vector.
    This function calculates the projection of vector `u` onto vector `v`.
    Parameters:
        - float u[2]: The vector to be projected
        - float v[2]: The vector onto which `u` is projected
    Returns:
        - float: The magnitude of the projection of `u` onto `v`
*/
float project_vector2(float u[2], float v[2]);

#endif