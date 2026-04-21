/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** IShape
*/

#ifndef INCLUDED_ISHAPE_HPP
    #define INCLUDED_ISHAPE_HPP

#include "Ray.hpp"
#include <iostream>
namespace RayTracer {

class IShape
{
    protected:
    public:
        virtual ~IShape() = default;
        virtual bool hits(const RayTracer::Ray& ray) const = 0;
};
}
#endif
