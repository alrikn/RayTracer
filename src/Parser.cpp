/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Parser
*/

#include "Parser.hpp"
#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "ILight.hpp"
#include "Scene.hpp"
#include "SpecularLight.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include <memory>

Parser::Parser()
{
    //we populate the factories:

    //shape factory
    shapeFactories["sphere"] = [this](const libconfig::Setting& shapeConfig) {
        return createSphere(shapeConfig);
    };
    shapeFactories["plane"] = [this](const libconfig::Setting& shapeConfig) {
        return createPlane(shapeConfig);
    };
    shapeFactories["rectangle"] = [this](const libconfig::Setting& shapeConfig) {
        return createRectangle(shapeConfig);
    };

    //light factory
    lightFactories["ambient"] = [this](const libconfig::Setting& lightConfig) {
        return createAmbientLight(lightConfig);
    };
    lightFactories["directional"] = [this](const libconfig::Setting& lightConfig) {
        return createDirectionalLight(lightConfig);
    };
    lightFactories["specular"] = [this](const libconfig::Setting& lightConfig) {
        return createSpecularLight(lightConfig);
    };
}

void Parser::run_parser(const std::string &filename)
{
    config.readFile(filename.c_str());

    parseScene();
    parseCamera();
    parseShapes();
    parseLights();
}

void Parser::parseScene()
{
    //for now hardcoded, in the future it won't be

    scene = std::make_unique<RayTracer::Scene>(0.9, 4); //for now hardcoded, in the future it won't be
}

void Parser::parseCamera()
{
    //for now hardcoded, in the future it won't be
    width = 4000; //this will be used to design better camera, so that the width and height of rectangle is based on this
    height = 2000;

    RayTracer::Camera camera(
        Math::Point3d(0, 1, 1), // move camera up and slightly back
    RayTracer::Rectangle(
        Math::Point3d(-3, -1.5, -1), // shift screen downward
        Math::Vector3d(6, 0, 0),
        Math::Vector3d(0, 3, 0)
        )
    );
    scene->setwidth(width);
    scene->setheight(height);
    scene->setCamera(camera);
}

void Parser::parseShapes()
{
    const libconfig::Setting& shapesConfig = config.lookup("shapes");

    for (int i = 0; i < shapesConfig.getLength(); i++) {
        const libconfig::Setting& shapeConfig = shapesConfig[i];

        std::string type;
        shapeConfig.lookupValue("type", type);

        auto it = shapeFactories.find(type);
        if (it != shapeFactories.end()) {
            std::shared_ptr<RayTracer::IShape> shape = it->second(shapeConfig);
            scene->addShape(shape);
        } else {
            std::cerr << "Unknown shape type: " << type << std::endl;
        }
    }
}

void Parser::parseLights()
{
    const libconfig::Setting& lightsConfig = config.lookup("lights");

    for (int i = 0; i < lightsConfig.getLength(); i++) {
        const libconfig::Setting& lightConfig = lightsConfig[i];

        std::string type;
        lightConfig.lookupValue("type", type);

        auto it = lightFactories.find(type);
        if (it != lightFactories.end()) {
            std::shared_ptr<RayTracer::ILight> light = it->second(lightConfig);
            scene->addLight(light);
        } else {
            std::cerr << "Unknown light type: " << type << std::endl;
        }
    }
}


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
    return rectangle;
}

//now for the light creation functions

std::shared_ptr<RayTracer::ILight> Parser::createAmbientLight(const libconfig::Setting& lightConfig)
{
    double brightness;
    lightConfig.lookupValue("brightness", brightness);

    auto ambientLight = std::make_shared<RayTracer::AmbientLight>(brightness);

    //if (lightConfig.exists("color")) {
    //    auto color = parseColor(lightConfig.lookup("color"));
    //    ambientLight->setColor(color);
    //}
    return ambientLight;
}

std::shared_ptr<RayTracer::ILight> Parser::createDirectionalLight(const libconfig::Setting& lightConfig)
{
    auto direction = parseVector3d(lightConfig.lookup("direction"));
    double brightness;
    lightConfig.lookupValue("brightness", brightness);

    auto directionalLight = std::make_shared<RayTracer::DirectionalLight>(direction, brightness);

    //if (lightConfig.exists("color")) {
    //    auto color = parseColor(lightConfig.lookup("color"));
    //    directionalLight->setColor(color);
    //}
    return directionalLight;
}

std::shared_ptr<RayTracer::ILight> Parser::createSpecularLight(const libconfig::Setting& lightConfig)
{
    auto direction = parseVector3d(lightConfig.lookup("direction"));
    double brightness;
    lightConfig.lookupValue("brightness", brightness);
    double shininess;
    lightConfig.lookupValue("shininess", shininess);
    double specular_strength;
    lightConfig.lookupValue("specular_strength", specular_strength);

    auto specularLight = std::make_shared<RayTracer::SpecularLight>(direction, brightness, shininess, specular_strength);

    //if (lightConfig.exists("color")) {
    //    auto color = parseColor(lightConfig.lookup("color"));
    //    specularLight->setColor(color);
    //}
    return specularLight;
}


/*the helper functions */

Math::Vector3d Parser::parseVector3d(const libconfig::Setting& array)
{
    if (array.getLength() != 3 || !array.isArray()) {
        throw std::runtime_error("Expected an array of length 3 for Vector3d");
    }
    double x = array[0];
    double y = array[1];
    double z = array[2];
    return Math::Vector3d(x, y, z);
}

Math::Point3d Parser::parsePoint3d(const libconfig::Setting& array)
{
    if (array.getLength() != 3 || !array.isArray()) {
        throw std::runtime_error("Expected an array of length 3 for Point3d");
    }
    double x = array[0];
    double y = array[1];
    double z = array[2];
    return Math::Point3d(x, y, z);
}

Color Parser::parseColor(const libconfig::Setting& string)
{
    std::string colorStr = string.c_str();
    //TODO: MAKE A MAP
    if (colorStr == "red") return RED;
    if (colorStr == "green") return GREEN;
    if (colorStr == "blue") return BLUE;
    if (colorStr == "white") return WHITE;
    if (colorStr == "black") return BLACK;
    if (colorStr == "yellow") return YELLOW;
    // Add more colors as needed
    std::cerr << "Unknown color: " << colorStr << ". Defaulting to white." << std::endl;
    return WHITE; // default color
}