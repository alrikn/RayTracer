/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** IShape
*/

#ifndef INCLUDED_ISHAPE_HPP
    #define INCLUDED_ISHAPE_HPP

#include "Ray.hpp"
#include "Vector3d.hpp"
#include <iostream>
namespace RayTracer {

//these be a few colors shapes can be at
enum Color {
    RED,
    GREEN,
    BLUE,
    WHITE
};

/*
this struct exists so that we can calculate the brightness of a point on a shape
*/
struct HitRecord {
    double distance; //this is the distance along the ray where the hit occurred
    Math::Point3d point; //the point of intersection
    Math::Vector3d normal; //the normal vector at the point of intersection, which is used for lighting calculations
    Math::Vector3d color; //the color of the shape at the point of intersection, which can be used for shading and rendering
};

class IShape
{
    protected:
    public:
        virtual ~IShape() = default;
        virtual bool hits(const RayTracer::Ray& ray) const = 0; //this should really return a hit record, but for now we can just return a bool to indicate if it hit or not
        virtual void setColor(Color color) = 0;
        virtual Color getColor() const = 0;
};
}
#endif
