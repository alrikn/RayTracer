/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Plane
*/

#ifndef INCLUDED_PLANE_HPP
    #define INCLUDED_PLANE_HPP

#include "Ashape.hpp"
#include "IShape.hpp"
#include "Ray.hpp"
#include "Vector3d.hpp"

namespace RayTracer {


class Plane : public Ashape
{
    private:
        Math::Vector3d normal; //the normal vector of the plane
        Math::Point3d point; //a point on the plane
        //double d; //the distance from the origin to the plane along the normal vector

    public:
        Plane(Math::Vector3d new_normal = Math::Vector3d(0, 1, 0), Math::Point3d point = Math::Point3d(0, 0, 0)) : normal(new_normal), point(point) {normal.normalize();};
        ~Plane() = default;

        std::optional<HitRecord> hits(const RayTracer::Ray& ray) const override;



};

}
#endif
