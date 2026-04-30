/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/


#include "Parser.hpp"
#include "Scene.hpp"



int main()
{

    Parser parser;

    try {
        parser.run_parser("scene_config.cfg");
    } catch (const std::exception& e) {
        std::cerr << "Error parsing scene configuration: " << e.what() << std::endl;
        return 84;
    }

    if (!parser.ParseSuccess()) {
        std::cerr << "Failed to parse the scene configuration." << std::endl;
        return 84;
    }

    RayTracer::Scene& scene = parser.getScene();

    scene.render(std::cout);
}
