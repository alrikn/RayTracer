/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"
#include "IShape.hpp"
#include "Ray.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include <cmath>


namespace RayTracer {

/*
R: x^2 + y^2 + z^2 = R^2 // Sphere equation. this measure the distance from the center of the sphere. here we assume that the center of the sphere is at the origin (0, 0, 0) to make it simpler
P: O + D * k = P // Ray equation, where O is the origin, D is the direction and k is the distance from the origin to the point of intersection

a point P(px, py, pz) is on the sphere if and only if:
px^2 + py^2 + pz^2 = R^2

we can rewrite the ray equation as:
{x = Ox + Dx * k
{y = Oy + Dy * k
{z = Oz + Dz * k

we can add the ray equation to the sphere equation:
{x = Ox + Dx * k
{y = Oy + Dy * k
{z = Oz + Dz * k
{x^2 + y^2 + z^2 = R^2

the goal is to find k such as

x^2 + y^2 + z^2 - R^2 = 0

//now we can gen an equation of the form:
A * k^2 + B *k + C = 0

this is a quadratic equation, we can solve it using the discriminant:
D = B^2 - 4 * A * C

*/
std::optional<HitRecord> Sphere::hits(const RayTracer::Ray& ray) const
{
    //first we have to shift the center
    //Math::Point3d oc_point = ray.origin - center;
    Math::Vector3d oc = ray.origin - center;

    Math::Vector3d d = ray.direction;
    double a = d.dot(d);
    double b = 2 * oc.dot(d);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = (b * b) - (4 * a * c);
    if (discriminant < 0) {
        return std::nullopt;
    }
    // if the discriminant is negative, it means that there is no real solution, which means that the ray does not hit the sphere. if the discriminant is zero, it means that there is one solution, which means that the ray is tangent to the sphere. if the discriminant is positive, it means that there are two solutions, which means that the ray hits the sphere at two points.
    //if the discriminant is zero, it means that there is one solution, which means that the ray is tangent to the sphere. if the discriminant is positive, it means that there are two solutions, which means that the ray hits the sphere at two points.


    double sqrtD = std::sqrt(discriminant);

    double k1 = (-b - sqrtD) / (2 * a);
    double k2 = (-b + sqrtD) / (2 * a);

    // pick the nearest valid one
    double k = k1;
    if (k < 0) {
        k = k2;
        if (k < 0) {
            return std::nullopt; // both behind the ray
        }
    }
    Math::Point3d point = ray.origin + ray.direction * k;

    Math::Vector3d normal = point - center;
    normal.normalize();

    HitRecord hit;
    hit.distance = k;
    hit.point = point;
    hit.normal = normal;
    hit.color = Math::Vector3d(255, 0, 0); // for now we can just return red for the color of the sphere. later we can add a color property to the sphere and return that instead
    return hit;
}
}
