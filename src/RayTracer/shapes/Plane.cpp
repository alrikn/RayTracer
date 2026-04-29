/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Plane
*/

#include "Plane.hpp"
#include "Vector3d.hpp"



namespace RayTracer {

std::optional<HitRecord> Plane::hits(const RayTracer::Ray& ray) const
{
    const double epsilon = 1e-6; //our new way of saying 0
    double denom = normal.dot(ray.direction);

    if (std::abs(denom) < epsilon)
        return std::nullopt; //ray is parallel to the plane

    double t = (origin - ray.origin).dot(normal) / denom;
    if (t < epsilon)
        return std::nullopt; //intersection is behind the ray origin

    Math::Vector3d final_normal = normal;
    if (denom > 0) {
        final_normal = normal * -1; //flip the normal if the ray is coming from the back side of the plane
    }

    HitRecord hit;
    hit.distance = t;
    hit.point = ray.origin + ray.direction * t;
    hit.normal = final_normal;
    hit.incomingDirection = ray.direction;
    return hit;
}
}