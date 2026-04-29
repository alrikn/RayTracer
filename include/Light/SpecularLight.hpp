/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SpecularLight
*/
#ifndef INCLUDED_SPECULARLIGHT_HPP
    #define INCLUDED_SPECULARLIGHT_HPP

#include "ALight.hpp"
#include "ILight.hpp"
#include "Vector3d.hpp"


namespace RayTracer {

class SpecularLight : public ALight
{
    private:
        Math::Vector3d _direction; //the direction the light is coming from
        double _light_intensity = 1.0; //how strong the light is overall
        Math::Vector3d _color = Math::Vector3d(255, 255, 255); //the color of the specular highlight (white by default)
        double _shininess = 32.0; //the specular exponent — higher value means a smaller, sharper highlight
        double _specular_strength = 0.5; //how much the specular highlight contributes to the final color
    public:
        SpecularLight(const Math::Vector3d &direction, double intensity = 1.0,
                      double shininess = 32.0, double specular_strength = 0.5,
                      const Math::Vector3d &color = Math::Vector3d(255, 255, 255));
        ~SpecularLight() = default;

        Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;
};
}
#endif
