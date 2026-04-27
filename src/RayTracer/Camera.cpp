/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** camera
*/

#include "Camera.hpp"


namespace RayTracer {
Ray Camera::ray(double u, double v) const
{
    //the point on the screen that the ray will hit is defined by the bottom left corner of the screen (screen.origin) plus the two sides of the screen (screen.bottom_side and screen.left_side) multiplied by the coordinates (u, v)
    Math::Point3d point_on_screen = screen.center + screen.bottom_side * u + screen.left_side * v;
    Math::Vector3d direction(point_on_screen.x - origin.x, point_on_screen.y - origin.y, point_on_screen.z - origin.z);
    return Ray(origin, direction); //the ray starts from the camera's origin and goes through the point on the screen defined by the coordinates (u, v)
}
}