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
        Math::Vector3d _color = Math::Vector3d(255, 255, 255); //white
    public:
        AmbientLight(double intensity, const Math::Vector3d& color = Math::Vector3d(255, 255, 255)) : _intensity(intensity), _color(color) {};
        ~AmbientLight() = default;

        Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;

};
}

#endif
