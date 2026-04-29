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
    try {
        config.readFile(filename.c_str());
    } catch (const libconfig::FileIOException &fioex) {
        std::cerr << "I/O error while reading file." << std::endl;
        return;
    } catch (const libconfig::ParseException &pex) {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return;
    }
    //config.readFile(filename.c_str()); //crashing immediately

    parseScene();
    //parseCamera();
    parseShapes();
    parseLights();
}

void Parser::parseScene()
{
    const libconfig::Setting& sceneConfig = config.lookup("scene");
    double brightness = 0.0;
    double max_depth = 0.0;

    brightness = parseDouble(sceneConfig.lookup("brightness"));
    max_depth = parseDouble(sceneConfig.lookup("max_depth"));

    double width = 4000; //default value, will be used to design better camera, so that the width and height of rectangle is based on this
    double height = 2000; //default value, will be used to design better camera, so that the width and height of rectangle is based on this

    width = parseDouble(sceneConfig.lookup("width"));
    height = parseDouble(sceneConfig.lookup("height"));

    scene = std::make_unique<RayTracer::Scene>(brightness, max_depth);

    RayTracer::Camera camera( //for now hardcoded. TODO: fix this in issue #10
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

//void Parser::parseCamera()
//{
//    //for now hardcoded, in the future it won't be
//    width = 4000; //this will be used to design better camera, so that the width and height of rectangle is based on this
//    height = 2000;
//
//    RayTracer::Camera camera(
//        Math::Point3d(0, 1, 1), // move camera up and slightly back
//    RayTracer::Rectangle(
//        Math::Point3d(-3, -1.5, -1), // shift screen downward
//        Math::Vector3d(6, 0, 0),
//        Math::Vector3d(0, 3, 0)
//        )
//    );
//    scene->setwidth(width);
//    scene->setheight(height);
//    scene->setCamera(camera);
//}

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


