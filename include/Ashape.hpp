/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Ashape
*/

#ifndef INCLUDED_ASHAPE_HPP
    #define INCLUDED_ASHAPE_HPP

#include <iostream>
#include "IShape.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace RayTracer {



class Ashape : public IShape
{
    private:
    protected:
    public:
        Ashape();
        Ashape(const Math::Point3d& center) : center(center) {};
        ~Ashape() = default;

        Math::Point3d center;

};

}
#endif
