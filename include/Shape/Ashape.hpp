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
        double _shininess = 32; //default shininess value, can be overridden by specific shapes
        double _specular_strength = 0.5; //default specular strength, can be overridden by specific shapes
    public:
        Ashape() = default;
        Ashape(const Math::Point3d& center) : center(center) {};
        ~Ashape() = default;

        Math::Point3d center;

        void setColor(Color color) override {
            this->default_color = color;
        }
        Color getColor() const override {
            return default_color;
        }
        void setShininess(double s) override {
            this->_shininess = s;
        }
        double getShininess() const override {
            return this->_shininess;
        }
        void setSpecularStrength(double s) override {
            this->_specular_strength = s;
        }
        double getSpecularStrength() const override {
            return this->_specular_strength;
        }
};

}
#endif
