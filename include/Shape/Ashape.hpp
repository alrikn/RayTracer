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
#include "Enums.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace RayTracer {



class Ashape : public IShape
{
    private:
    protected:
        Color default_color = RED;
        ShapeType shape_type;
    public:
        Ashape() = default;
        Ashape(const Math::Point3d& origin, ShapeType type) : shape_type(type), origin(origin) {};

        ~Ashape() = default;

        Math::Point3d origin;
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
            if (reflectivity < 0.0)
                reflectivity = 0.0;
            if (reflectivity > 1.0)
                reflectivity = 1.0;
            this->reflectivity = reflectivity;
        }
        ShapeType getShapeType() const override {
            return shape_type;
        }
};

}
#endif
