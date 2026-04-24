/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Matrix
*/

#include "Matrix.hpp"
#include "IShape.hpp"
#include "Vector3d.hpp"
#include <limits>

namespace RayTracer {
void Matrix::addObject(const std::shared_ptr<IShape> &object)
{
    _objects.push_back(object);
}

void Matrix::addLight(const std::shared_ptr<ILight> &light)
{
    _lights.push_back(light);

}

Math::Vector3d Matrix::traceRay(const Ray &ray) const
{
    return traceRay(ray, 0);
}

double clamp_color(double x)
{
    return std::max(0.0, std::min(255.0, x));
}


Math::Vector3d Matrix::traceRay(const Ray &ray, int depth) const
{
    std::optional<HitRecord> closest_hit; //we only care about first thing it hit (imagine a wall. don't care what behind wall)
    double closest_distance = std::numeric_limits<double>::infinity();


    for (const auto& object : _objects) {
        auto hit = object->hits(ray);
        if (hit.has_value() && (hit->distance > epsilon) && (hit->distance < closest_distance)) {
            hit->color = COLOR_MAP.at(object->getColor()); //set the color of the hit record to the color of the object that was hit
            closest_hit = hit;
            closest_distance = hit->distance;
        }
    }

    if (!closest_hit.has_value()) {
        return Math::Vector3d(0, 0, 0); //black background if we hit nothing
    }
    //when we have a vector of light libs well loop over them but for now
    Math::Vector3d light_contribution(0, 0, 0);
    for (const auto& light : _lights)
        light_contribution += light->intensity(*closest_hit, _objects);

    Math::Vector3d object_color; //combine the object color with the light contribution to get the final color at the hit point
    //now we clamp it to 255 proportinally
    object_color = light_contribution * (closest_hit->color / 255.0) * brightness;

    if (depth >= max_depth) {
        return object_color; //return the light contribution if we've reached the maximum recursion depth
    }

    //here we calculate the angle of the reflected ray
    Math::Vector3d incoming_ray_dir = ray.direction;
    incoming_ray_dir.normalize();
    Math::Vector3d normal = closest_hit->normal;
    normal.normalize();
    Math::Vector3d reflected_dir = incoming_ray_dir - normal * 2 * incoming_ray_dir.dot(normal); //calculate the reflected ray direction (making it bounce)
    reflected_dir.normalize();

    //now we create the reflected ray and trace it to get the color contribution from the reflected ray
    Ray reflected_ray(closest_hit->point + normal * epsilon, reflected_dir); //offset
    Math::Vector3d reflected_color = traceRay(reflected_ray, depth + 1); //trace the reflected ray and get its color contribution

    //we really need a reflectivity value for the objects but oh well

    //TODO; find smth better instead of an addirion here.
    Math::Vector3d result_color = object_color + (reflected_color * 0.5); //combine the object color and the reflected color contribution (0.5 to avoid making everything a mirror)
    result_color = Math::Vector3d(clamp_color(result_color.x), clamp_color(result_color.y), clamp_color(result_color.z));
    return (result_color); //combine the object color and the reflected color contribution
}

void write_color(const Math::Vector3d &color, std::ostream &output)
{
    int r = static_cast<int>(color.x);
    int g = static_cast<int>(color.y);
    int b = static_cast<int>(color.z);
    if (r > 255 || g > 255 || b > 255)
        throw std::runtime_error("ERROR: color value out of range = " + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b));
    output << r << " " << g << " " << b << "\n";
}

void Matrix::render(const Camera &camera, int width, int height, std::ostream &output) const
{
    output << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            double u = static_cast<double>(i) / (width - 1);
            double v = static_cast<double>(j) / (height - 1);
            Ray ray = camera.ray(u, v);
            Math::Vector3d color = traceRay(ray);
            write_color(color, output);
        }
    }
}
}