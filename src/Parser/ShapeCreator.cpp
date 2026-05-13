/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** ShapeCreator
*/

#include "Cylinder.hpp"
#include "IShape.hpp"
#include "Parser.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Cone.hpp"
#include <memory>

/*now for the creation functions */


std::shared_ptr<RayTracer::IShape> Parser::createSphere(const libconfig::Setting& shapeConfig)
{
    auto origin = parsePoint3d(shapeConfig.lookup("origin"));
    double radius;
    shapeConfig.lookupValue("radius", radius);

    auto sphere = std::make_shared<RayTracer::Sphere>(origin);
    sphere->setRadius(radius);

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

    auto plane = std::make_shared<RayTracer::Plane>(point);
    plane->setNormal(normal);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        plane->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        plane->setReflectivity(reflectivity);
    }
    if (shapeConfig.exists("chessboard_pattern")) {
        bool chessboard_pattern = parseBool(shapeConfig.lookup("chessboard_pattern"));
        plane->setChessboardPattern(chessboard_pattern);
    }
    return plane;
}

std::shared_ptr<RayTracer::IShape> Parser::createRectangle(const libconfig::Setting& shapeConfig)
{
    auto origin = parsePoint3d(shapeConfig.lookup("origin"));
    auto bottom_side = parseVector3d(shapeConfig.lookup("bottom_side"));
    auto left_side = parseVector3d(shapeConfig.lookup("left_side"));

    auto rectangle = std::make_shared<RayTracer::Rectangle>(origin);
    rectangle->setBottomSide(bottom_side).setLeftSide(left_side);

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

std::shared_ptr<RayTracer::IShape> Parser::createCylinder(const libconfig::Setting& shapeConfig)
{
    auto center = parsePoint3d(shapeConfig.lookup("origin"));
    auto axis = parseVector3d(shapeConfig.lookup("axis"));
    double radius = parseDouble(shapeConfig.lookup("radius"));
    double height = parseDouble(shapeConfig.lookup("height"));

    auto cylinder = std::make_shared<RayTracer::Cylinder>(center);
    cylinder->setAxis(axis).setRadius(radius).setHeight(height);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        cylinder->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        cylinder->setReflectivity(reflectivity);
    }
    return cylinder;
}

std::shared_ptr<RayTracer::IShape> Parser::createCone(const libconfig::Setting& shapeConfig)
{
    auto center = parsePoint3d(shapeConfig.lookup("origin"));
    auto axis = parseVector3d(shapeConfig.lookup("axis"));
    double radius = parseDouble(shapeConfig.lookup("radius"));
    double height = parseDouble(shapeConfig.lookup("height"));

    auto cone = std::make_shared<RayTracer::Cone>(center);
    cone->setAxis(axis).setRadius(radius).setHeight(height);

    if (shapeConfig.exists("color")) {
        auto color = parseColor(shapeConfig.lookup("color"));
        cone->setColor(color);
    }
    if (shapeConfig.exists("reflectivity")) {
        double reflectivity = parseDouble(shapeConfig.lookup("reflectivity"));
        cone->setReflectivity(reflectivity);
    }
    return cone;
}
