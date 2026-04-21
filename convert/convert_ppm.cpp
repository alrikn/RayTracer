/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** convert_ppm
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION

extern "C" {
    #include "stb_image_write.h"
}

int main(int argc, char *argv[])
{
    std::string output_filename = "output.png";
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ppm> [output.png]\n";
        return 84;
    }
    if (argc >= 3) {
        output_filename = argv[2];
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open \"" << argv[1] << "\" file\n";
        return 84;
    }

    std::string format;
    file >> format;
    if (format != "P3") {
        std::cerr << "Only P3 format supported\n";
        return 84;
    }

    int width, height, maxval;
    file >> width >> height >> maxval;

    std::vector<unsigned char> image(width * height * 3);

    for (int i = 0; i < width * height * 3; ++i) {
        int value;
        file >> value;
        image[i] = static_cast<unsigned char>(
            (value * 255) / maxval  // normalize if maxval != 255
        );
    }

    if (!stbi_write_png(output_filename.c_str(), width, height, 3,
                        image.data(), width * 3)) {
        std::cerr << "Failed to write PNG\n";
        return 84;
    }

    std::cout << "Conversion successful!\n";
    return 0;
}