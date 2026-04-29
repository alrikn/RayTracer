/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** LightCreator
*/

#include "AmbientLight.hpp"
#include "DirectionalLight.hpp"
#include "Parser.hpp"
#include "SpecularLight.hpp"


//now for the light creation functions

std::shared_ptr<RayTracer::ILight> Parser::createAmbientLight(const libconfig::Setting& lightConfig)
{
    double brightness;
    lightConfig.lookupValue("brightness", brightness);

    auto ambientLight = std::make_shared<RayTracer::AmbientLight>(brightness);

    if (lightConfig.exists("color_vector")) {
        auto color = parseVector3d(lightConfig.lookup("color_vector"));
        ambientLight = std::make_shared<RayTracer::AmbientLight>(brightness, color);
    }
    return ambientLight;
}

std::shared_ptr<RayTracer::ILight> Parser::createDirectionalLight(const libconfig::Setting& lightConfig)
{
    auto direction = parseVector3d(lightConfig.lookup("direction"));
    double brightness;
    lightConfig.lookupValue("brightness", brightness);

    auto directionalLight = std::make_shared<RayTracer::DirectionalLight>(direction, brightness);

    if (lightConfig.exists("color_vector")) {
        auto color = parseVector3d(lightConfig.lookup("color_vector"));
        directionalLight = std::make_shared<RayTracer::DirectionalLight>(direction, brightness, color);
    }
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

    if (lightConfig.exists("color_vector")) {
        auto color = parseVector3d(lightConfig.lookup("color_vector"));
        specularLight = std::make_shared<RayTracer::SpecularLight>(direction, brightness, shininess, specular_strength, color);
    }
    return specularLight;
}
