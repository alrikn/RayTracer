/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cylinder.cpp
*/

#include "Cylinder.hpp"
#include "IShape.hpp"
#include <optional>
#include <cmath>

namespace RayTracer {

/*
The cylinder is defined by its base center, its axis and its radius. the axis is a vector that defines the direction of the cylinder. the height is the distance from the base to the top of the cylinder.
To find the intersection of a ray with a cylinder, we can use the following steps:
1. We can find the intersection of the ray with the infinite cylinder defined by the axis and the radius. this will give us two points of intersection, one on the top and one on the bottom of the cylinder.
2. We can then check if the points of intersection are within the height of the cylinder. if they are, then we have a valid intersection. if they are not, then we can check if the ray intersects with the caps of the cylinder. the caps are defined by
the base center and the top center of the cylinder. if the ray intersects with the caps, then we have a valid intersection. if the ray does not intersect with the caps, then we have no intersection.
The equation of the infinite cylinder can be derived from the equation of a circle in 2D. the equation of a circle is:
(x - cx)^2 + (y - cy)^2 = r^2
where (cx, cy) is the center of the circle and r is the radius. we can rewrite this equation in 3D by replacing x and y with the coordinates of the point of
intersection and the center of the circle with the base center of the cylinder. we also need to take into account the direction of the cylinder, which is defined by the axis. we can do this by projecting the point of intersection onto the axis and subtracting this projection from the point of intersection. this will give us a vector that is perpendicular to the axis and points from the base center to the point of intersection. we can then use this vector to find the distance from the point of intersection to the axis, which should be equal to the radius of the cylinder for a valid intersection.
The equation of the infinite cylinder can be written as:
((P - C) - ((P - C) . A) * A)^2 = r^2
where P is the point of intersection, C is the base center of the cylinder, A is the axis of the cylinder and r is the radius of the cylinder. we can rewrite
this equation as:
((P - C) - ((P - C) . A) * A)^2 -
r^2 = 0
we can then substitute the ray equation into this equation to find the values of k that satisfy this equation. the ray equation is:
P = O + D * k
where O is the origin of the ray, D is the direction of the ray and k is the distance from the origin to the point of intersection. we can substitute this equation into the equation of the infinite cylinder to get:
(((O + D * k) - C) - (((O + D * k) - C) . A) * A)^2 -
r^2 = 0
*/

std::optional<HitRecord> Cylinder::check_cylinder_hit(double discriminant, double a, double b, const RayTracer::Ray& ray) const
{
    std::optional<HitRecord> hit = std::nullopt;

    double sqrtD = std::sqrt(discriminant);
    double k1 = (-b - sqrtD) / (2 * a);
    double k2 = (-b + sqrtD) / (2 * a);
    double k = std::numeric_limits<double>::infinity();
    if (k1 > 0 && k1 < k)
        k = k1;
    if (k2 > 0 && k2 < k)
        k = k2;
    if (k == std::numeric_limits<double>::infinity())
        return hit; // no valid intersection
    if (k >= 0) {
        Math::Point3d intersection = ray.origin + (ray.direction * k);
        double height_at_intersection = (intersection - origin).dot(axis);
        if (height_at_intersection >= 0 && height_at_intersection <= height) {
            HitRecord record;
            record.point = intersection;
            record.normal = (intersection - (origin + axis * height_at_intersection)).normalize();
            record.color = COLOR_MAP.at(getColor());
            record.incomingDirection = ray.direction;
            record.distance = k;
            hit = record;
        }
    }
    return hit;
}

std::optional<HitRecord> Cylinder::check_front_cap_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_cap) const
{
    Math::Point3d cap_intersection = ray.origin + (ray.direction * t_cap);
    if ((cap_intersection - origin).dot(cap_intersection - origin) <= radius * radius) {
        if (!hit || t_cap < hit->distance) {
            HitRecord record;
            record.point = cap_intersection;
            record.normal = axis * -1;
            record.color = COLOR_MAP.at(getColor());
            record.incomingDirection = ray.direction;
            record.distance = t_cap;
            hit = record;
        }
    }
    return hit;
}

std::optional<HitRecord> Cylinder::check_back_cap_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_cap, const Math::Point3d& top_center) const
{
    Math::Point3d cap_intersection = ray.origin + (ray.direction * t_cap);
    if ((cap_intersection - top_center).dot(cap_intersection - top_center) <= radius * radius) {
        if (!hit || t_cap < hit->distance) {
            HitRecord record;
            record.point = cap_intersection;
            record.normal = axis;
            record.color = COLOR_MAP.at(getColor());
            record.incomingDirection = ray.direction;
            record.distance = t_cap;
            hit = record;
        }
    }
    return hit;
}

std::optional<HitRecord> Cylinder::hits(const RayTracer::Ray& ray) const //the problem with the current implementat
{
    std::optional<HitRecord> hit = std::nullopt;
    Math::Vector3d oc = ray.origin - origin;
    double oc_proj = oc.dot(axis);
    Math::Vector3d d_perp = ray.direction - (axis * ray.direction.dot(axis));
    Math::Vector3d oc_perp = oc - (axis * oc_proj);
    double a = d_perp.dot(d_perp);
    double b = 2 * d_perp.dot(oc_perp);   // no extra term needed
    double c = oc_perp.dot(oc_perp) - (radius * radius);

    double discriminant = (b * b) - (4 * a * c);

    //if the discriminant is negative, it means that there is no real solution, which means that the ray does not hit the infinite cylinder. if the discriminant is zero, it means that there is one solution, which means that the ray is tangent to the infinite cylinder. if the discriminant is positive, it means that there are two solutions, which means that the ray hits the infinite cylinder at two points.
    if (discriminant >= 0) {
        hit = check_cylinder_hit(discriminant, a, b, ray);
    }
    //front cap intersection.
    double denom = ray.direction.dot(axis);
    if (std::abs(denom) < 1e-6) {
        return hit; // ray is parallel to the caps, no need to check them
    }
    double t_cap = (origin - ray.origin).dot(axis) / denom;
    if (t_cap > 0) {
        hit = check_front_cap_hit(ray, hit, t_cap);
    }
    //back cap intersection
    Math::Point3d top_center = origin + (axis * height);
    t_cap = (top_center - ray.origin).dot(axis) / denom;
    if (t_cap > 0) {
        hit = check_back_cap_hit(ray, hit, t_cap, top_center);
    }
    return hit;
}
/*builder design pattern*/

Cylinder& Cylinder::setAxis(const Math::Vector3d& axis) {
    this->axis = axis.normalize();
    return *this;
}

Cylinder& Cylinder::setRadius(double radius) {
    this->radius = radius;
    return *this;
}

Cylinder& Cylinder::setHeight(double height) {
    this->height = height;
    return *this;
}
}
