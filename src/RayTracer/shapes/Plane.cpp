/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Plane
*/

#include "Plane.hpp"
#include "Vector3d.hpp"
#include <cmath>



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
    hit.color = COLOR_MAP.at(getColor());
    hit.incomingDirection = ray.direction;
    if (!chessboard_pattern)
        return hit; //if we don't have a chessboard pattern, we can return the hit record as is


    //now that we know for sure that we hit the plane we can check for checkerboard

    int checker_size = 1; //size of the checker squares
    int x = static_cast<int>(std::floor(hit.point.x / checker_size));
    int z = static_cast<int>(std::floor(hit.point.z / checker_size));
    if ((x + z) % 2 == 0) {
        hit.color = COLOR_MAP.at(getColor()); //color of the plane
    } else {
        hit.color = Math::Vector3d(255, 255, 255) - COLOR_MAP.at(getColor()); //inverse color for the checker pattern
    }
    return hit;
}
}