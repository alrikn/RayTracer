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
    _light = light;

}

Math::Vector3d Matrix::traceRay(const Ray &ray) const
{
    return traceRay(ray, 0);
}

Math::Vector3d Matrix::traceRay(const Ray &ray, int depth) const
{
    const int max_depth = 4; //max recursion
    const double epsilon = 1e-4; //small value to prevent selfintersection

    std::optional<HitRecord> closest_hit;
    double closest_distance = std::numeric_limits<double>::infinity();


    for (const auto& object : _objects) {
        auto hit = object->hits(ray);
        if (hit.has_value() && (hit->distance > epsilon) && (hit->distance < closest_distance)) {
            hit->color = COLOR_MAP.at(object->getColor()); //set the color of the hit record to the color of the object that was hit
            //rn we are not using this because the _light is perfroming calculations on it
            closest_hit = hit;
            closest_distance = hit->distance;
        }
    }

    if (!closest_hit.has_value()) {
        return Math::Vector3d(0, 0, 0); //black background if we hit nothing
    }

    //when we have a vector of light libs well loop over them but for now
    Math::Vector3d light_contribution = _light->intensity(*closest_hit, _objects); //get the light contribution from the light source

    Math::Vector3d object_color = light_contribution; //combine the object color with the light contribution to get the final color at the hit point
    if (depth >= max_depth) {
        return object_color; //return the light contribution if we've reached the maximum recursion depth
    }

    //here we calculate the angle of the reflected ray
    Math::Vector3d incoming_ray_dir = ray.direction;
    incoming_ray_dir.normalize();
    Math::Vector3d normal = closest_hit->normal;
    normal.normalize();
    Math::Vector3d reflected_dir = incoming_ray_dir - normal * 2 * incoming_ray_dir.dot(normal); //calculate the reflected ray direction
    reflected_dir.normalize();

    //now we create the reflected ray and trace it to get the color contribution from the reflected ray
    Ray reflected_ray(closest_hit->point + normal * epsilon, reflected_dir); //offset
    Math::Vector3d reflected_color = traceRay(reflected_ray, depth + 1); //trace the reflected ray and get its color contribution

    //we really need a reflectivity value for the objects but oh well

    //maybe its a + here not sure
    return (object_color + reflected_color); //combine the object color and the reflected color contribution
}

void Matrix::render(const Camera &camera, int width, int height, std::ostream &output) const
{
    output << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            double u = static_cast<double>(i) / (width - 1);
            double v = static_cast<double>(j) / (height - 1);
            Ray ray = camera.ray(u, v);
            Math::Vector3d color = traceRay(ray);
            output << static_cast<int>(color.x) << " " << static_cast<int>(color.y) << " " << static_cast<int>(color.z) << "\n";
        }
    }
}
}