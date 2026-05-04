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
#include "Point3d.hpp"
#include "Scene.hpp"
#include "SpecularLight.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include <cmath>
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
    shapeFactories["cylinder"] = [this](const libconfig::Setting& shapeConfig) {
        return createCylinder(shapeConfig);
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
    parseCamera();
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

    scene->setwidth(width);
    scene->setheight(height);
}

void Parser::parseCamera()
{
    unsigned int scene_width = scene->getWidth();
    unsigned int scene_height = scene->getHeight();
    const libconfig::Setting& cameraConfig = config.lookup("camera");

    Math::Point3d origin = parsePoint3d(cameraConfig.lookup("origin"));
    Math::Vector3d direction = parseVector3d(cameraConfig.lookup("direction"));
    double zoom = parseDouble(cameraConfig.lookup("zoom"));
    // --- 1. Normalize forward direction ---
    Math::Vector3d forward = direction.normalize();

    // --- 2. Build orthonormal basis (right, up) ---
    Math::Vector3d worldUp(0, 1, 0);

    // Handle edge case: camera looking almost straight up/down
    if (fabs(forward.dot(worldUp)) > 0.999)
        worldUp = Math::Vector3d(0, 0, 1);

    Math::Vector3d right = forward.cross(worldUp).normalize();
    Math::Vector3d up = right.cross(forward).normalize();

    // --- 3. Aspect ratio ---
    double aspect = static_cast<double>(scene_width) /
                    static_cast<double>(scene_height);

    // --- 4. Screen size from zoom ---
    // (zoom = "how close the screen is", bigger zoom = smaller screen)
    double screenHeight = 1.0 / zoom;
    double screenWidth = screenHeight * aspect;

    // --- 5. Distance from camera to screen ---
    double focalDistance = 1.0;

    Math::Point3d center = origin + forward * focalDistance;

    // --- 6. Rectangle axes ---
    Math::Vector3d horizontal = right * screenWidth;
    Math::Vector3d vertical   = up * screenHeight;

    // --- 7. Bottom-left corner of the screen ---
    Math::Point3d bottomLeft =
        center - (horizontal / 2.0) - (vertical / 2.0);

    // --- 8. Build camera ---
    RayTracer::Rectangle screen(
        bottomLeft,
        horizontal,
        vertical
    );

    RayTracer::Camera camera(origin, screen);
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


