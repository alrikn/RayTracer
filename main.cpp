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


int main()
{
    int x_axis = 400;
    int y_axis = 200;

    std::cout << "P3\n" << x_axis << " " << y_axis << "\n255\n"; //ppm header
    RayTracer::Camera cam;
    RayTracer::Sphere s(Math::Point3d(0, 0, -1), 0.5);
    for (int j = 0; j < y_axis; j++) {
        for (int i = 0; i < x_axis; i++) {
            double u = (double)i / (x_axis - 1);
            double v = (double)j / (y_axis - 1);
            RayTracer::Ray r = cam.ray(u, v);
            if (s.hits(r)) {
                std::cout << "0 0 255\n";
            } else {
                std::cout << "255 255 255\n";
            }
        }
    }
}