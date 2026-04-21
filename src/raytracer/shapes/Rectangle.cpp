/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Rectangle
*/

#include "Rectangle.hpp"
#include "IShape.hpp"
#include <optional>


//a rectangle is technically not a shape, but we treat it as one

namespace RayTracer {
std::optional<HitRecord> Rectangle::hits(const RayTracer::Ray& ray) const
{
    return std::nullopt; //todo later
}
}