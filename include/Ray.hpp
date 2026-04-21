/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Ray
*/

#ifndef INCLUDED_RAY_HPP
    #define INCLUDED_RAY_HPP

#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace RayTracer {


class Ray
{
    private:
    protected:
    public:
        Ray();
        Ray(const Math::Point3d &origin, const Math::Vector3d &direction) : origin(origin), direction(direction) {};
        ~Ray() = default;

        Math::Point3d origin;
        Math::Vector3d direction;

};

}
#endif
