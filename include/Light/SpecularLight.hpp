/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SpecularLight
*/
#ifndef INCLUDED_SPECULARLIGHT_HPP
    #define INCLUDED_SPECULARLIGHT_HPP

#include "ILight.hpp"
#include "Vector3d.hpp"


namespace RayTracer {

class SpecularLight : public ILight
{
    private:
        Math::Vector3d _direction;
        double _intensity = 1.0;
        Math::Vector3d _color = Math::Vector3d(255, 255, 255);
        double _shininess = 32.0;
        double _specular_strength = 0.5;
    public:
        SpecularLight(const Math::Vector3d &direction, double intensity = 1.0,
                      double shininess = 32.0, double specular_strength = 0.5,
                      const Math::Vector3d &color = Math::Vector3d(255, 255, 255));
        ~SpecularLight() = default;

        Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;
};
}
#endif
