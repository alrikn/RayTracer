/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.hpp
*/

#ifndef INCLUDED_CYLINDER_HPP
    #define INCLUDED_CYLINDER_HPP
#include "Ashape.hpp"
#include "IShape.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include "Ray.hpp"
#include "Enums.hpp"
#include <optional>

namespace RayTracer {
class Cylinder : public Ashape
{
    private:
    protected:
    public:
        Math::Vector3d axis;
        double radius = 0;
        double height = 0;

        //Cylinder();
        Cylinder(const Math::Point3d &center, const Math::Vector3d &axis, double radius, double height) : Ashape(center, CYLINDER), axis(axis.normalize()), radius(radius), height(height) {};
        ~Cylinder() = default;


        std::optional<HitRecord> hits(const RayTracer::Ray& ray) const override;
        //helper funcs
        std::optional<HitRecord> check_cylinder_hit(double discriminant, double a, double b, const RayTracer::Ray& ray) const;

        std::optional<HitRecord> check_front_cap_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_cap) const;
        std::optional<HitRecord> check_back_cap_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_cap, const Math::Point3d& top_center) const;
};

}

#endif