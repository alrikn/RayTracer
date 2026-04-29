/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** DirectionalLight
*/

#include "DirectionalLight.hpp"
#include "ALight.hpp"
#include "Vector3d.hpp"


namespace RayTracer {

DirectionalLight::DirectionalLight(const Math::Vector3d &new_direction, double light_intensity, const Math::Vector3d &color) :
    ALight(light_intensity, color)
{
    this->direction = new_direction.normalize();
}

Math::Vector3d DirectionalLight::intensity(const RayTracer::HitRecord& hit, const std::vector<std::shared_ptr<IShape>> &objects) const
{
    //epsilon 
    const double epsilon = 1e-4; //small value to prevent selfintersection
    //here we flip the direction (light source so it points back to the light)
    Math::Vector3d light_dir = (direction * -1).normalize(); //direction from the hit point to the light source

    double lambert = std::max(hit.normal.dot(light_dir), 0.0); //lambertian reflectance
    //lambert just means that it checks if a surface is facing towards the light source or not. if 1 then perfectly facing, if 0 then perfectly facing away.
    if (lambert <= 0.0) {
        return Math::Vector3d(0, 0, 0); //no light contribution if the surface is facing away from the light
    }

    //now we loop through all the objects to check if the light (that is going to the sun) is blocked by anyone. if it is then shadow.
    RayTracer::Ray shadow_ray(hit.point + hit.normal * epsilon, light_dir); //ray from the hit point to the light source, offset by epsilon to prevent self-intersection
    for (const auto& object : objects) {
        auto shadow_hit = object->hits(shadow_ray);
        if (shadow_hit.has_value() && shadow_hit->distance > epsilon) {
            return Math::Vector3d(0, 0, 0); //in shadow, no light contribution
        }
    }
    return _color * _light_intensity * lambert * (hit.color / 255.0); //return the light contribution based on the color, intensity, and lambertian reflectance

}
}