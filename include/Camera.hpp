/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Camera
*/

#ifndef INCLUDED_CAMERA_HPP
    #define INCLUDED_CAMERA_HPP

#include "Point3d.hpp"
#include "Ray.hpp"
#include "Rectangle.hpp"
#include <iostream>

namespace RayTracer {

class Camera
{
    private:
    protected:
    public:
        Camera() {screen = Rectangle(Math::Point3d(-2, -1, -1), Math::Vector3d(4, 0, 0), Math::Vector3d(0, 2, 0));};
        Camera(const Math::Point3d& origin, const Rectangle& screen) : origin(origin), screen(screen) {};
        ~Camera() = default;

        Math::Point3d origin;
        Rectangle screen; //the screen is a rectangle that represents the area where the rays will hit. it is defined by its bottom left corner (origin) and its two sides (bottom_side and left_side). the bottom_side represents the vector from the bottom left corner to the bottom right corner, and the left_side represents the vector from the bottom left corner to the top left corner.
        Ray ray(double u, double v) const; //returns a ray that starts from the camera's origin and goes through the point on the screen defined by the coordinates (u, v). u and v are between 0 and 1, where (0, 0) is the bottom left corner of the screen and (1, 1) is the top right corner of the screen.
};

}
#endif
