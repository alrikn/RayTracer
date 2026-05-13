/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Rectangle
*/

#include "Rectangle.hpp"
#include "IShape.hpp"
#include <optional>


namespace RayTracer {
std::optional<HitRecord> Rectangle::hits(const RayTracer::Ray& ray) const
{
    //rectangle normal is the cross product of the two sides
    Math::Vector3d normal = bottom_side.cross(left_side).normalize();

    //is ray parallel to the rectangle plane?
    double denom = normal.dot(ray.direction);

    const double EPSILON = 1e-8;

    if (std::abs(denom) < EPSILON) {
        return std::nullopt; //no intersect, parallel
    }

    //find t (t is the distance along the ray where the intersection occurs)
    double t = (origin - ray.origin).dot(normal) / denom;

    //behind ray origin, no hit
    if (t < 0) {
        return std::nullopt;
    }

    //calculate the actual intersection point
    Math::Point3d point = ray.origin + ray.direction * t;

    //now we have to check if the point is inside the rectangle bounds.
    //we do this by projecting the vector from the rectangle origin to the point onto the two sides of the rectangle and checking if the projections are between 0 and the length of the sides.
    Math::Vector3d local = point - origin;

    // Compute rectangle coordinates
    double u = local.dot(bottom_side) / bottom_side.dot(bottom_side);

    double v = local.dot(left_side) / left_side.dot(left_side);

    //ot recrangle bounds, no hit
    if (u < 0.0 || u > 1.0 ||
        v < 0.0 || v > 1.0) {
        return std::nullopt;
    }

    //we flip the normal if it's facing the wrong way (if the ray is coming from the back side of the rectangle)
    if (normal.dot(ray.direction) > 0) {
        normal = normal * -1;
    }

    HitRecord hit;
    hit.distance = t;
    hit.point = point;
    hit.normal = normal;
    hit.color = COLOR_MAP.at(getColor());
    hit.incomingDirection = ray.direction;

    return hit;
}
}