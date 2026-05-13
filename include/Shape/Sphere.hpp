/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Sphere
*/

#ifndef INCLUDED_SPHERE_HPP
    #define INCLUDED_SPHERE_HPP

#include "Ashape.hpp"
#include "IShape.hpp"
#include "Point3d.hpp"
#include "Ray.hpp"
#include <optional>
namespace RayTracer {



class Sphere : public Ashape
{
    private:
    protected:
    public:
        Sphere(const Math::Point3d &center) : Ashape(center, SPHERE) {};
        //Sphere(const Math::Point3d &center, double radius) : Ashape(center, ShapeType::SPHERE), radius(radius) {};
        ~Sphere() = default;

        double radius = 0;

        std::optional<HitRecord> hits(const RayTracer::Ray& ray) const override;

        /*builder design pattern*/
        Sphere& setRadius(double radius);

};
}

#endif
