/*
** EPITECH PROJECT, 2025
** Raytracer
** File description:
** Cone.cpp
*/

#include "Cone.hpp"
#include "IShape.hpp"
#include <cmath>
#include <optional>

/*
To check if a ray intersects with a cone, we can use the following steps:
1. We can find the intersection of the ray with the infinite cone defined by the axis and the angle of the cone. this will give us two points of intersection, one on the top and
one on the bottom of the cone.
2. We can then check if the points of intersection are within the height of the cone. if they are, then we have a valid intersection. if they are not, then we can check
if the ray intersects with the base of the cone. the base is defined by the center of the cone and the radius of the base. if the ray intersects with the base, then we have a valid intersection. if the ray does not intersect with the base, then we have no intersection.
The equation of the infinite cone can be derived from the equation of a circle in 2D. the equation of a circle is:
(x - cx)^2 + (y - cy)^2 = r^2
where (cx, cy) is the center of the circle and r is the radius. we can rewrite this equation in 3D by replacing x and y with the coordinates
of the point of intersection and the center of the circle with the center of the cone. we also need to take into account the direction of the cone, which is defined by the axis. we can do this by projecting the point of intersection onto the axis and subtracting this projection from the point of intersection. this will give us a vector that is perpendicular to the axis and points from the center of the cone to the point of intersection. we can then use this vector to find the distance from the point of intersection to the axis, which should be equal to the radius of the cone at that height for a valid intersection.
The equation of the infinite cone can be written as:
((P - C) - ((P - C) . A) * A)^2 = (r/h)^2 * ((P - C) . A)^2
where P is the point of intersection, C is the center of the cone, A is the axis of the cone,r is the radius of the base of the cone and h is the height of the cone. we
can rewrite this equation as:
((P - C) - ((P - C) . A) * A)^2 - (r/h)^2 * ((P - C) . A)^2 = 0
we can then substitute the ray equation into this equation to find the values of k that satisfy this
equation. the ray equation is:
P = O + D * k
where O is the origin of the ray, D is the direction of the ray and k is the distance from the origin to the point of intersection. we can substitute this equation into the equation of
the infinite cone to get:
(((O + D * k) - C) - (((O + D * k) - C) . A) * A)^2 - (r/h)^2 * (((O + D * k) - C) . A)^2 = 0
*/

std::optional<RayTracer::HitRecord> RayTracer::Cone::check_cone_hit(double discriminant, double a, double b, const RayTracer::Ray& ray) const
{
    std::optional<HitRecord> hit = std::nullopt;
    double sqrtD = std::sqrt(discriminant);
    double k1 = (-b - sqrtD) / (2 * a);
    double k2 = (-b + sqrtD) / (2 * a);

    if (k1 > 0) {
        Math::Point3d intersection = ray.origin + (ray.direction * k1);
        double height_at_intersection = (intersection - origin).dot(axis);
        if (height_at_intersection >= 0 && height_at_intersection <= height) {
            HitRecord record;
            Math::Vector3d radial = (intersection - (origin + axis * height_at_intersection)).normalize();
            record.point = intersection;
            record.normal = (radial * height - axis * radius).normalize();
            record.color = COLOR_MAP.at(getColor());
            record.incomingDirection = ray.direction;
            record.distance = k1;
            hit = record;
        }
    }
    if (k2 > 0 && (!hit || k2 < hit->distance)) {
        Math::Point3d intersection = ray.origin + (ray.direction * k2);
        double height_at_intersection = (intersection - origin).dot(axis);
        if (height_at_intersection >= 0 && height_at_intersection <= height) {
            HitRecord record;
            Math::Vector3d radial = (intersection - (origin + axis * height_at_intersection)).normalize();
            record.point = intersection;
            record.normal = (radial * height - axis * radius).normalize();
            record.color = COLOR_MAP.at(getColor());
            record.incomingDirection = ray.direction;
            record.distance = k2;
            hit = record;
        }
    }
    return hit;
}

std::optional<RayTracer::HitRecord>RayTracer::Cone::check_base_hit(const RayTracer::Ray& ray, std::optional<HitRecord> hit, double t_base) const
{
    Math::Point3d intersection = ray.origin + (ray.direction * t_base);
    Math::Point3d base_center = origin + axis * height;
    Math::Vector3d to_hit = intersection - base_center;
    double dist_from_center = to_hit.dot(to_hit) - std::pow(to_hit.dot(axis), 2);
    if (dist_from_center <= radius * radius) {
        HitRecord record;
        record.point = intersection;
        record.normal = axis;
        record.color = COLOR_MAP.at(getColor());
        record.incomingDirection = ray.direction;
        record.distance = t_base;
        if (!hit || t_base < hit->distance) {
            hit = record;
        }
    }
    return hit;
}


//it doesnt work. this is because the equation of the cone is not correct. we need to take into account the height of the cone and the radius of the base of the cone. we also need to take into account the direction of the cone, which is defined by the axis. we can do this by projecting the point of intersection onto the axis and subtracting this projection from the point of intersection. this will give us a vector that is perpendicular to the axis and points from the center of the cone to the point of intersection. we can then use this vector to find the distance from the point of intersection to the axis, which should be equal to the radius of the cone at that height for a valid intersection.
//the correct function is:
std::optional<RayTracer::HitRecord>RayTracer::Cone::hits(const RayTracer::Ray& ray) const
{
    std::optional<HitRecord> hit = std::nullopt;
    Math::Vector3d oc = ray.origin - origin;
    double cos_theta = height / std::sqrt(radius * radius + height * height);
    double cos_theta_squared = cos_theta * cos_theta;
    double oc_dot_axis = oc.dot(axis);
    double d_dot_axis = ray.direction.dot(axis);
    double a = d_dot_axis * d_dot_axis - cos_theta_squared * ray.direction.dot(ray.direction);
    double b = 2.0 * (oc_dot_axis * d_dot_axis - cos_theta_squared * ray.direction.dot(oc));
    double c = oc_dot_axis * oc_dot_axis - cos_theta_squared * oc.dot(oc);
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant >= 0) {
        hit = check_cone_hit(discriminant, a, b, ray);
    }

    double t_base = (origin + axis * height - ray.origin).dot(axis) / ray.direction.dot(axis);
    if (t_base >= 0) {
        hit = check_base_hit(ray, hit, t_base);
    }
    
    return hit;
}