/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/


#include "Parser.hpp"
#include "Scene.hpp"

void help_message()
{
    std::cout << "USAGE: ./raytracer <SCENE_FILE>" << std::endl;
    std::cout << "SCENE_FILE: path to the scene configuration file (a .cfg file)" << std::endl;
}

int main(int argc, char **argv)
{

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_config_file>" << std::endl;
        return 84;
    }
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
