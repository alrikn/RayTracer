/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/

#include "AmbientLight.hpp"
#include "Camera.hpp"
#include "Matrix.hpp"
#include "Rectangle.hpp"
#include "Ashape.hpp"
#include "Sphere.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include <memory>

void write_color(const Math::Vector3d &color)
{
    int r = static_cast<int>(color.x);
    int g = static_cast<int>(color.y);
    int b = static_cast<int>(color.z);
    std::cout << r << " " << g << " " << b << "\n";
}

int main()
{
    int x_axis = 4000;
    int y_axis = 2000;

    RayTracer::Matrix scene;
    std::shared_ptr<RayTracer::Sphere> sphere1 = std::make_shared<RayTracer::Sphere>(Math::Point3d(0, 0, -1), 0.5);
    sphere1->setColor(RayTracer::RED);
    scene.addObject(sphere1);

    scene.addLight(std::make_shared<RayTracer::AmbientLight>(0.5));

    scene.render(RayTracer::Camera(), x_axis, y_axis, std::cout);
}