/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/

#include "AmbientLight.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "IShape.hpp"
#include "Scene.hpp"
#include "Plane.hpp"
#include "Rectangle.hpp"
#include "Ashape.hpp"
#include "SpecularLight.hpp"
#include "Sphere.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include <memory>


//push test
//testing made entirely by gibbidy. sue me
void testing_func()
{
    int x_axis = 8000;
    int y_axis = 4000;

    RayTracer::Scene scene(0.9, 4); //brightness, max_depth

    // --- Center sphere ---
    auto center = std::make_shared<RayTracer::Sphere>(
        Math::Point3d(0, 0, -1), 0.5);
    center->setColor(RayTracer::RED);
    scene.addObject(center);

    // --- Left sphere ---
    auto left = std::make_shared<RayTracer::Sphere>(
        Math::Point3d(-1.0, 0, -1.5), 0.5);
    left->setColor(RayTracer::BLUE);
    scene.addObject(left);

    // --- Right sphere ---
    auto right = std::make_shared<RayTracer::Sphere>(
        Math::Point3d(1.0, 0, -1.5), 0.5);
    right->setColor(RayTracer::GREEN);
    scene.addObject(right);

    // --- Small sphere (closer) ---
    auto small = std::make_shared<RayTracer::Sphere>(
        Math::Point3d(0.3, -0.3, -0.5), 0.2);
    small->setColor(RayTracer::YELLOW);
    scene.addObject(small);

    auto ground = std::make_shared<RayTracer::Plane>(
        Math::Vector3d(0, 1, 0),      // upward normal
        Math::Point3d(0, -0.5, 0)   // point on plane
    );
    ground->setColor(RayTracer::WHITE);
    ground->setReflectivity(0.15); //make the ground slightly less reflective
    scene.addObject(ground);

    // --- Light ---
    scene.addLight(std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3d(-1, -1, -1), 0.8));
    scene.addLight(std::make_shared<RayTracer::SpecularLight>(
        Math::Vector3d(-1, -1, -1), 1.0, 128.0, 1.0));
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(0.8));

    //Camera() {screen = Rectangle(Math::Point3d(-2, -1, -1), Math::Vector3d(4, 0, 0), Math::Vector3d(0, 2, 0));};
    //Camera(const Math::Point3d& origin, const Rectangle& screen) : origin(origin), screen(screen) {};

    RayTracer::Camera camera(
        Math::Point3d(0, 1, 1), // move camera up and slightly back
    RayTracer::Rectangle(
        Math::Point3d(-3, -1.5, -1), // shift screen downward
        Math::Vector3d(6, 0, 0),
        Math::Vector3d(0, 3, 0)
        )
    );

    // --- Render ---
    scene.render(camera, x_axis, y_axis, std::cout);
}

int main()
{
    //to be replaced with the parser
    testing_func();
}
