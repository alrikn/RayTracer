/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Rectangle
*/

#ifndef INCLUDED_RECTANGLE_HPP
    #define INCLUDED_RECTANGLE_HPP

#include "Ashape.hpp"
#include "Vector3d.hpp"
namespace RayTracer {


class Rectangle : public Ashape
{
    private:
    protected:
    public:
        Rectangle() = default;
        Rectangle(const Math::Point3d &origin, const Math::Vector3d &bottom_side, const Math::Vector3d &left_side) : Ashape(origin), bottom_side(bottom_side), left_side(left_side) {};
        ~Rectangle() = default;

        //here the origin is actually the bottom left
        Math::Vector3d bottom_side; //represents the vector from bottom left corner to bottom right corner
        Math::Vector3d left_side; //represents the vector from bottom left corner to top left corner

        bool hits(const RayTracer::Ray& ray) const override;
};
}

#endif
