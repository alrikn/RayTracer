/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Sphere
*/

#ifndef INCLUDED_SPHERE_HPP
    #define INCLUDED_SPHERE_HPP

#include "Ashape.hpp"
#include "Point3d.hpp"
#include "Ray.hpp"
namespace RayTracer {



class Sphere : public Ashape
{
    private:
    protected:
    public:
        Sphere();
        Sphere(const Math::Point3d &center, double radius) : Ashape(center), radius(radius) {};
        ~Sphere() = default;

        double radius = 0;

        bool hits(const RayTracer::Ray& ray) const override;

};
}

#endif
