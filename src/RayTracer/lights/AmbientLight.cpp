/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** AmbientLight
*/

#include "AmbientLight.hpp"
namespace RayTracer {



Math::Vector3d AmbientLight::intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const
{
    (void)hit; //avoid compiler warning
    if (_intensity <= 0.0 || objects.empty()) { //mainly for compiler warning
        return Math::Vector3d(0, 0, 0);
    }
    return _color * _intensity; //the ambient light contributes a constant amount to the lighting of the scene, regardless of the position or orientation of the surface being shaded. the color of the ambient light is determined by the _color member variable, and the intensity is determined by the _intensity member variable. we simply multiply these together to get the final contribution of the ambient light to the lighting of the scene.
}
}
