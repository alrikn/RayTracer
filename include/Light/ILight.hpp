/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Ilight
*/

#ifndef INCLUDED_ILIGHT_HPP
    #define INCLUDED_ILIGHT_HPP

#include "IShape.hpp"
#include <memory>
#include <vector>

namespace RayTracer {


class ILight
{
    protected:
    public:
        virtual ~ILight() = default;
        //the intensity should take as input the point being shaded, the normal at that point, and the direction to the light source
        virtual Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const = 0;

};
}
#endif
