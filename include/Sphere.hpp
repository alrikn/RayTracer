/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Sphere
*/

#ifndef INCLUDED_SPHERE_HPP
    #define INCLUDED_SPHERE_HPP

#include "Point3d.hpp"
#include "Ray.hpp"
namespace RayTracer {



class Sphere
{
    private:
    protected:
    public:
        Sphere();
        Sphere(const Math::Point3d &center, double radius) : center(center), radius(radius) {};
        ~Sphere() = default;

        Math::Point3d center;
        double radius = 0;

        bool hits(RayTracer::Ray ray) const;

};
}

#endif
