/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** DirectionalLight
*/

#ifndef INCLUDED_DIRECTIONALLIGHT_HPP
    #define INCLUDED_DIRECTIONALLIGHT_HPP

#include <iostream>
#include <memory>
#include <vector>
#include "ILight.hpp"
#include "IShape.hpp"
#include "Vector3d.hpp"

namespace RayTracer {



class DirectionalLight : public ILight
{
    private:
        Math::Vector3d direction; //this is where the light comes from (the sun in a way)
        double light_intensity = 1; //how strong the light is
        Math::Vector3d color = Math::Vector3d(255, 255, 255); //the color of the light (white by default)
    protected:
    public:
        DirectionalLight(const Math::Vector3d &direction, double light_intensity = 1, const Math::Vector3d &color = Math::Vector3d(255, 255, 255));

        ~DirectionalLight() = default;

        Math::Vector3d intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const override;

};
}


#endif
