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
        Color default_color = RED;
    public:
        Ashape() = default;
        Ashape(const Math::Point3d& center) : center(center) {};
        ~Ashape() = default;

        Math::Point3d center;
        double reflectivity = 0.5; //how much the object reflects light. 0 means no reflection, 1 means perfect mirror. this is a value between 0 and 1.

        void setColor(Color color) override {
            this->default_color = color;
        }
        Color getColor() const override {
            return default_color;
        }
        double getReflectivity() const override {
            return reflectivity;
        }
        void setReflectivity(double reflectivity) override {
            this->reflectivity = reflectivity;
        }
};

}
#endif
