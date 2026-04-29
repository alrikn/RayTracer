/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Parser
*/

#ifndef INCLUDED_PARSER_HPP
    #define INCLUDED_PARSER_HPP

#include "Enums.hpp"
#include "IShape.hpp"
#include "Scene.hpp"
#include <functional>
#include <libconfig.h++>
#include <map>
#include <memory>



/*
SHAPES:
    sphere:
    - origin (point3d) OBLIGATORY
    - radius (double) OBLIGATORY
    - color (color) OPTIONAL
    - reflectibity (double) OPTIONAL

    plane:
    - normal (vector3d) OBLIGATORY
    - point (point3d) OBLIGATORY //might be optional in future haven't decided
    - color (color) OPTIONAL
    - reflectibity (double) OPTIONAL

    rectangle:
    - origin (point3d) OBLIGATORY
    - bottom_side (vector3d) OBLIGATORY
    - left_side (vector3d) OBLIGATORY
    - color (color) OPTIONAL
    - reflectibity (double) OPTIONAL

LIGHTS:
    ambient:
    - brightness (double) OBLIGATORY
    - color (vector3d) OPTIONAL //for the lights its a vector3d because i want the user to have more control

    directional:
    - direction (vector3d) OBLIGATORY
    - brightness (double) OBLIGATORY
    - color (vector3d) OPTIONAL

    specular:
    - direction (vector3d) OBLIGATORY
    - brightness (double) OBLIGATORY
    - shininess (double) OBLIGATORY
    - specular_strength (double) OBLIGATORY
    - color (vector3d) OPTIONAL
*/

using ShapeFactoryFunc = std::function<std::shared_ptr<RayTracer::IShape>(const libconfig::Setting& shapeConfig)>;

using LightFactoryFunc = std::function<std::shared_ptr<RayTracer::ILight>(const libconfig::Setting& lightConfig)>;

class Parser
{
    private:
        std::unique_ptr<RayTracer::Scene> scene = nullptr; //the scene that will be rendered. the parser will fill this scene with objects and lights based on the input file
        libconfig::Config config; //the config object
        std::map<std::string, ShapeFactoryFunc> shapeFactories;
        std::map<std::string, LightFactoryFunc> lightFactories;
        void parseScene();
        void parseShapes();
        void parseLights();

        //now the funcs to populate the shapeFactories map.
        std::shared_ptr<RayTracer::IShape> createSphere(const libconfig::Setting& shapeConfig);
        std::shared_ptr<RayTracer::IShape> createPlane(const libconfig::Setting& shapeConfig);
        std::shared_ptr<RayTracer::IShape> createRectangle(const libconfig::Setting& shapeConfig);
        std::shared_ptr<RayTracer::IShape> createCylinder(const libconfig::Setting& shapeConfig);

        //now the funcs to populate the lightFactories map.
        std::shared_ptr<RayTracer::ILight> createAmbientLight(const libconfig::Setting& lightConfig);
        std::shared_ptr<RayTracer::ILight> createDirectionalLight(const libconfig::Setting& lightConfig);
        std::shared_ptr<RayTracer::ILight> createSpecularLight(const libconfig::Setting& lightConfig);


        //a few helper funcs for parsing:
        Math::Vector3d parseVector3d(const libconfig::Setting& array);
        Math::Point3d parsePoint3d(const libconfig::Setting& array);
        Color parseColor(const libconfig::Setting& string);
        double parseDouble(const libconfig::Setting& setting);
    public:
        Parser();


        void run_parser(const std::string &filename);
        RayTracer::Scene& getScene() { return *scene; }
        bool ParseSuccess() const { return scene != nullptr; }
        ~Parser() = default;

};


/*

*/

#endif
