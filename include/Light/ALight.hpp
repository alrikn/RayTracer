/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ALight
*/

#ifndef INCLUDED_ALIGHT_HPP
    #define INCLUDED_ALIGHT_HPP

    #include "ILight.hpp"
    #include "Vector3d.hpp"
namespace RayTracer {


class ALight : public ILight
{
    private:
    protected:
        double _light_intensity = 1.0; //the intensity of the light, which is a value between 0 and 1 that represents how much the light contributes to the overall lighting of the scene. a value of 0 means that the light has no effect, while a value of 1 means that the light contributes fully to the lighting of the scene.
        Math::Vector3d _color = Math::Vector3d(255, 255, 255); //the color of the light, which is represented as a vector3d where each component (x, y, z) corresponds to the red, green, and blue channels of the light respectively. each component is a value between 0 and 255 that represents the intensity of that color channel in the light.
    public:
        ALight(double intensity, const Math::Vector3d& color) : _light_intensity(intensity), _color(color) {};

        ~ALight() = default;

};
}

#endif
