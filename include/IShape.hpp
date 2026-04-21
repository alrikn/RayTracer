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
#include <optional>
#include <unordered_map>
namespace RayTracer {

//these be a few colors shapes can be at
enum Color {
    RED,
    GREEN,
    BLUE,
    WHITE
};

//colour fromshape:
const Math::Vector3d RED_COLOR(255, 0, 0);
const Math::Vector3d GREEN_COLOR(0, 255, 0);
const Math::Vector3d BLUE_COLOR(0, 0, 255);
const Math::Vector3d WHITE_COLOR(255, 255, 255);

//we make an unoredered map to convert from the enum to the actual color vector
const std::unordered_map<Color, Math::Vector3d> COLOR_MAP = {
    {RED, RED_COLOR},
    {GREEN, GREEN_COLOR},
    {BLUE, BLUE_COLOR},
    {WHITE, WHITE_COLOR}
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

        //optional allows us to return nullptr if no hit
        virtual std::optional<HitRecord> hits(const RayTracer::Ray& ray) const = 0; //this should really return a hit record, but for now we can just return a bool to indicate if it hit or not
        virtual void setColor(Color color) = 0;
        virtual Color getColor() const = 0;
};
}
#endif
