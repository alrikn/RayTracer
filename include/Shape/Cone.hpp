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
#include <optional>

namespace RayTracer {
class Cone : public Ashape
{
    private:
    protected:
    public:
        Math::Vector3d axis;
        double radius = 0;
        double height = 0;

        Cone(const Math::Point3d &center) : Ashape(center, CONE) {};
        //Cone(const Math::Point3d &center, const Math::Vector3d &axis, double radius, double height) : Ashape(center, CONE), axis(axis.normalize()), radius(radius), height(height) {};
        ~Cone() = default;
        std::optional<HitRecord> hits(const RayTracer::Ray& ray) const override;
        std::optional<HitRecord> check_cone_hit(double discriminant, double a, double b, const RayTracer::Ray& ray) const;
        std::optional<HitRecord> check_base_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_base) const;

        /*builder design pattern*/
        Cone& setAxis(const Math::Vector3d& axis);
        Cone& setRadius(double radius);
        Cone& setHeight(double height);
};
}
#endif