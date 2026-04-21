/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Ilight
*/

#ifndef INCLUDED_ILIGHT_HPP
    #define INCLUDED_ILIGHT_HPP

#include "IShape.hpp"
#include <iostream>

namespace RayTracer {


class ILight
{
    protected:
    public:
        virtual ~ILight() = default;
        //the intensity should take as input the point being shaded, the normal at that point, and the direction to the light source
        virtual double intensity(RayTracer::HitRecord hit) = 0;
};
}
#endif
