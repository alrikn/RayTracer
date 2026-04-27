/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Scene
*/

#include "Scene.hpp"
#include "IShape.hpp"
#include "Vector3d.hpp"
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

namespace RayTracer {
void Scene::addObject(const std::shared_ptr<IShape> &object)
{
    _objects.push_back(object);
}

void Scene::addLight(const std::shared_ptr<ILight> &light)
{
    _lights.push_back(light);

}

Math::Vector3d Scene::traceRay(const Ray &ray) const
{
    return traceRay(ray, 0);
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


    for (const auto& object : _objects) {
        auto hit = object->hits(ray);
        if (hit.has_value() && (hit->distance > epsilon) && (hit->distance < closest_distance)) {
            hit->color = COLOR_MAP.at(object->getColor()); //set the color of the hit record to the color of the object that was hit
            closest_hit = hit;
            closest_distance = hit->distance;
            hit->incomingDirection = ray.direction; //set the ray direction in the hit record for later use in lighting calculations
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

    if (depth >= max_depth) {
        return light_at_point; //return the light contribution if we've reached the maximum recursion depth
    }

    //here we calculate the angle of the reflected ray
    Math::Vector3d incoming_ray_dir = ray.direction.normalize(); //normalize the incoming ray direction
    Math::Vector3d normal = closest_hit->normal.normalize();
    Math::Vector3d reflected_dir = (incoming_ray_dir - normal * 2 * incoming_ray_dir.dot(normal)).normalize(); //calculate the reflected ray direction (making it bounce)

    //now we create the reflected ray and trace it to get the color contribution from the reflected ray
    Ray reflected_ray(closest_hit->point + normal * epsilon, reflected_dir); //offset
    Math::Vector3d reflected_color = traceRay(reflected_ray, depth + 1); //trace the reflected ray and get its color contribution

    //we really need a reflectivity value for the objects but oh well

    //TODO; find smth better instead of an addirion here.
    Math::Vector3d result_color = light_at_point + (reflected_color * 0.5); //combine the object color and the reflected color contribution (0.5 to avoid making everything a mirror)
    result_color = Math::Vector3d(clamp_color(result_color.x), clamp_color(result_color.y), clamp_color(result_color.z));
    return (result_color); //combine the object color and the reflected color contribution
}

void Scene::write_color(const Math::Vector3d &color, std::string &output) const
{
    int r = static_cast<int>(color.x);
    int g = static_cast<int>(color.y);
    int b = static_cast<int>(color.z);
    if (r > 255 || g > 255 || b > 255)
        throw std::runtime_error("ERROR: color value out of range = " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b));
    output += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + "\n";
}

void Scene::render(const Camera &camera, int width, int height, std::ostream &output) const
{
    std::string _final_output = "P3\n" + std::to_string(width) + " " + std::to_string(height) + "\n255\n";

    int percentage = 0;
    int new_percentage = 0;
    for (int j = height - 1; j >= 0; j--) {
        new_percentage = (height - j) * 100 / height;
        if (new_percentage != percentage) {
            percentage = new_percentage;
            std::cerr << "\rRendering: " << percentage << "%";
        }

        for (int i = 0; i < width; i++) {
            double u = static_cast<double>(i) / (width - 1);
            double v = static_cast<double>(j) / (height - 1);
            Ray ray = camera.ray(u, v);
            Math::Vector3d color = traceRay(ray);
            write_color(color, _final_output);
        }
    }
    output << _final_output;
}

}