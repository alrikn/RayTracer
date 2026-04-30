/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** raytracing
*/

#include "Scene.hpp"
#include "IShape.hpp"
#include "Vector3d.hpp"
#include <limits>
#include <memory>
#include <vector>

#include <array>
#include <cstdint>
#include <algorithm>

namespace RayTracer {
void Scene::addShape(const std::shared_ptr<IShape> &object)
{
    _objects.push_back(object);
}

void Scene::addLight(const std::shared_ptr<ILight> &light)
{
    _lights.push_back(light);

}

double Scene::clamp_color(double x) const
{
    return std::max(0.0, std::min(255.0, x));
}

Math::Vector3d Scene::average_light(std::vector<Math::Vector3d> light_contributions) const
{
    if (light_contributions.empty()) {
        return Math::Vector3d(0, 0, 0);
    }
    Math::Vector3d sum(0, 0, 0);
    for (const auto& contribution : light_contributions) {
        sum += contribution;
    }
    sum.x = sum.x / static_cast<double>(light_contributions.size());
    sum.y = sum.y / static_cast<double>(light_contributions.size());
    sum.z = sum.z / static_cast<double>(light_contributions.size());
    return sum;
}

Math::Vector3d Scene::traceRay(const Ray &ray, int depth) const
{
    std::optional<HitRecord> closest_hit; //we only care about first thing it hit (imagine a wall. don't care what behind wall)
    double closest_distance = std::numeric_limits<double>::infinity();
    IShape* closest_object = nullptr; //we need this to get the color of the object that was hit

    for (const auto& object : _objects) {
        auto hit = object->hits(ray);
        if (hit.has_value() && (hit->distance > epsilon) && (hit->distance < closest_distance)) {
            closest_hit = hit;
            closest_distance = hit->distance;
            closest_object = object.get();
        }
    }

    if (!closest_hit.has_value()) {
        return Math::Vector3d(0, 0, 0); //black background if we hit nothing
    }
    //when we have a vector of light libs well loop over them but for now

    Math::Vector3d light_contribution = Math::Vector3d(0, 0, 0);
    std::vector<Math::Vector3d> light_contributions;
    for (const auto& light : _lights) {
        light_contributions.push_back(light->intensity(*closest_hit, _objects));
    }
    light_contribution = average_light(light_contributions);

    Math::Vector3d light_at_point = light_contribution * brightness;

    if (depth >= max_depth || closest_object->getReflectivity() <= 0.0) {
        return light_at_point; //return the light contribution if we've reached the maximum recursion depth
    }

    //here we calculate the angle of the reflected ray
    Math::Vector3d incoming_ray_dir = ray.direction.normalize(); //normalize the incoming ray direction
    Math::Vector3d normal = closest_hit->normal.normalize();
    Math::Vector3d reflected_dir = (incoming_ray_dir - normal * 2 * incoming_ray_dir.dot(normal)).normalize(); //calculate the reflected ray direction (making it bounce)

    //now we create the reflected ray and trace it to get the color contribution from the reflected ray
    Ray reflected_ray(closest_hit->point + normal * epsilon, reflected_dir); //offset
    Math::Vector3d reflected_light = traceRay(reflected_ray, depth + 1); //trace the reflected ray and get its color contribution

    //we really need a reflectivity value for the objects but oh well

    //TODO; find smth better instead of an addirion here.
    Math::Vector3d final_light = light_at_point + (reflected_light * closest_object->getReflectivity()); //combine the object color and the reflected color contribution (0.5 to avoid making everything a mirror)
    final_light = Math::Vector3d(clamp_color(final_light.x), clamp_color(final_light.y), clamp_color(final_light.z));
    return (final_light); //combine the object color and the reflected color contribution
}

void Scene::write_color(const std::array<uint8_t, 3> &color, std::string &output) const
{
    output += std::to_string(color[0]) + " " + std::to_string(color[1]) + " " + std::to_string(color[2]) + "\n";
}


}
