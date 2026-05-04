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

    aaFactories["supersampling"] = [this](const libconfig::Setting &cfg) {
        return createSupersampling(cfg);
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
    parseAntiAliasing();
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

    if (width <= 0 || height <= 0 || brightness < 0 || max_depth < 0) {
        throw std::runtime_error("Scene width, height, brightness, and max_depth must be positive values.");
    }

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
    //
    Math::Vector3d forward = direction.normalize();

    //we build the orthonormal basis for the camera, 
    // which is used to calculate the screen rectangle. 
    // we need to find two vectors that are perpendicular to the forward vector and to each other. 
    // we can use the cross product to find these vectors. 
    // we also need to handle the case where the forward vector is parallel to the world up vector,
    //  in which case we can use a different world up vector 
    // to avoid getting a zero vector from the cross product.
    Math::Vector3d worldUp(0, 1, 0);

    // Handle edge case: camera looking almost straight up/down
    if (fabs(forward.dot(worldUp)) > 0.999)
        worldUp = Math::Vector3d(0, 0, 1);

    Math::Vector3d right = forward.cross(worldUp).normalize();
    Math::Vector3d up = right.cross(forward).normalize();

    //the aspect ratio of screen is based on scene width height, so that its not distorted
    double aspect = static_cast<double>(scene_width) /
                    static_cast<double>(scene_height);

    //da zoom
    // (zoom = "how close the screen is", bigger zoom = smaller screen)
    double screenHeight = 1.0 / zoom;
    double screenWidth = screenHeight * aspect;

    //distance from camera to screen
    double focalDistance = 1.0;

    Math::Point3d center = origin + (forward * focalDistance);

    //rectangle axes
    Math::Vector3d horizontal = right * screenWidth;
    Math::Vector3d vertical   = up * screenHeight;

    //bottom left corner of the screen rectangle.
    // we calculate this by starting at the center of the screen and then moving left by half the horizontal vector and down by half the vertical vector.
    Math::Point3d bottomLeft =
        center - (horizontal / 2.0) - (vertical / 2.0);

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

void Parser::parseAntiAliasing()
{
    const libconfig::Setting &sceneConfig = config.lookup("scene");
    if (!sceneConfig.exists("antialiasing"))
        return;

    const libconfig::Setting &aaCfg = sceneConfig.lookup("antialiasing");
    std::string technique;
    if (!aaCfg.lookupValue("technique", technique))
        throw std::runtime_error("[Antialiasing] Missing required 'technique' key.");

    auto it = aaFactories.find(technique);
    if (it == aaFactories.end()) {
        std::string available = "[Antialiasing] Unknown technique: " + technique + "\nAvailable techniques:\n";
        for (auto jt = aaFactories.begin(); jt != aaFactories.end(); ++jt)
            available += "  - " + jt->first + "\n";
        throw std::runtime_error(available);
    }

    scene->setAA(it->second(aaCfg));
}


