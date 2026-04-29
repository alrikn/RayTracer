/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** AmbientLight
*/

#ifndef INCLUDED_AMBIENTLIGHT_HPP
    #define INCLUDED_AMBIENTLIGHT_HPP

#include "ALight.hpp"
#include "ILight.hpp"
namespace RayTracer {



class AmbientLight : public ALight
{
    private:
    public:
        AmbientLight(double intensity, const Math::Vector3d& color = Math::Vector3d(255, 255, 255)) : ALight(intensity, color) {};
        ~AmbientLight() = default;

        Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;

};
}

#endif
