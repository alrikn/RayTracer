/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/

#include "Camera.hpp"
#include "Rectangle.hpp"
#include "Ashape.hpp"
#include "Sphere.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"

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

    std::cout << "P3\n" << x_axis << " " << y_axis << "\n255\n"; //ppm header
    RayTracer::Camera cam;
    RayTracer::Sphere s(Math::Point3d(0, 0, -1), 0.5);
    for (int j = 0; j < y_axis; j++) {
        for (int i = 0; i < x_axis; i++) {
            double u = (double)i / (x_axis - 1);
            double v = (double)j / (y_axis - 1);
            RayTracer::Ray r = cam.ray(u, v);
            if (s.hits(r)) {
                write_color(Math::Vector3d(255, 0, 0));
            } else {
                //if it didn't hit jack then its black innit?
                write_color(Math::Vector3d(0, 0, 0));
            }
        }
    }
}