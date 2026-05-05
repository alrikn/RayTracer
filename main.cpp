/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** main
*/


#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <memory>

#include "Parser.hpp"
#include "Scene.hpp"

int help_message()
{
    std::cerr << "USAGE: ./raytracer <SCENE_FILE>" << std::endl;
    std::cerr << "\tSCENE_FILE: path to the scene configuration file (a .cfg file)" << std::endl;
    std::cerr << "OPTIONAL Flags:" << std::endl;
    std::cerr << "\t-o <OUTPUT_FILE>: path to the output image file (a .ppm file)" << std::endl;
    return 84;
}

/*
if they put the -o flag, instead of printing the ppm to stdout, we will write it to the specified file
*/
std::string check_output_file(int argc, char **argv)
{
    for (int i = 1; i < argc - 1; i++) {
        if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            return std::string(argv[i + 1]);
        }
    }
    return "";
}

std::ostream& parse_input(int argc, char **argv, std::ofstream &file)
{
    if (argc < 2) {
        exit(help_message());
    }

    std::string outputFile = check_output_file(argc, argv);
    if (!outputFile.empty()) {
        file.open(outputFile);
        if (!file) {
            std::cerr << "Error opening output file: " << outputFile << std::endl;
            exit(help_message());
        }
        return file;
    }

    return std::cout;
}

int main(int argc, char **argv)
{

    if (argc < 2) {
        return help_message();
    }
    Parser parser;

    try {
        parser.run_parser(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing scene configuration: " << e.what() << std::endl;
        return 84;
    }

    if (!parser.ParseSuccess()) {
        std::cerr << "Failed to parse the scene configuration." << std::endl;
        return 84;
    }

    RayTracer::Scene& scene = parser.getScene();
    std::ofstream outputFile;
    std::ostream& output = parse_input(argc, argv, outputFile);
    scene.render(output);
}
