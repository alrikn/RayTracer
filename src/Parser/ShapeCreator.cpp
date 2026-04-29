/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ShapeCreator
*/

#include "Parser.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"

/*now for the creation functions */


std::shared_ptr<RayTracer::IShape> Parser::createSphere(const libconfig::Setting& shapeConfig)
{
    auto origin = parsePoint3d(shapeConfig.lookup("origin"));
    double radius;
    shapeConfig.lookupValue("radius", radius);

    auto sphere = std::make_shared<RayTracer::Sphere>(origin, radius);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        sphere->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        sphere->setReflectivity(reflectivity);
    }
    return sphere;
}

std::shared_ptr<RayTracer::IShape> Parser::createPlane(const libconfig::Setting& shapeConfig)
{
    auto normal = parseVector3d(shapeConfig.lookup("normal"));
    auto point = parsePoint3d(shapeConfig.lookup("point"));

    auto plane = std::make_shared<RayTracer::Plane>(normal, point);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        plane->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        plane->setReflectivity(reflectivity);
    }
    return plane;
}

std::shared_ptr<RayTracer::IShape> Parser::createRectangle(const libconfig::Setting& shapeConfig)
{
    auto origin = parsePoint3d(shapeConfig.lookup("origin"));
    auto bottom_side = parseVector3d(shapeConfig.lookup("bottom_side"));
    auto left_side = parseVector3d(shapeConfig.lookup("left_side"));

    auto rectangle = std::make_shared<RayTracer::Rectangle>(origin, bottom_side, left_side);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        rectangle->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        rectangle->setReflectivity(reflectivity);
    }
    return rectangle;
}