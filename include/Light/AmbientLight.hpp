/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** AmbientLight
*/

#ifndef INCLUDED_AMBIENTLIGHT_HPP
    #define INCLUDED_AMBIENTLIGHT_HPP

#include "ILight.hpp"
namespace RayTracer {



class AmbientLight : public ILight
{
    private:
        double _intensity = 1.0; //the intensity of the ambient light, which is a value between 0 and 1 that represents how much the ambient light contributes to the overall lighting of the scene. a value of 0 means that the ambient light has no effect, while a value of 1 means that the ambient light contributes fully to the lighting of the scene.
    protected:
    public:
        AmbientLight(double intensity) : _intensity(intensity) {};
        ~AmbientLight() = default;

        double intensity(RayTracer::HitRecord hit) override;

};
}

#endif
