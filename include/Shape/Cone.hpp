/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.hpp
*/

#ifndef INCLUDED_CONE_HPP
    #define INCLUDED_CONE_HPP
#include "Ashape.hpp"
#include "IShape.hpp"
#include "Point3d.hpp"

namespace RayTracer {
class Cone : public Ashape
{
    private:
    protected:
    public:
        Math::Vector3d axis;
        double radius = 0;
        double height = 0;

        Cone();
        Cone(const Math::Point3d &center, const Math::Vector3d &axis, double radius, double height) : Ashape(center, CYLINDER), axis(axis), radius(radius), height(height) {};
        ~Cone() = default;
        std::optional<HitRecord> hits(const RayTracer::Ray& ray) const override;
};
}
#endif