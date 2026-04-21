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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.ppm>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::string format;
    file >> format;
    if (format != "P3") {
        std::cerr << "Only P3 format supported\n";
        return 1;
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

    if (!stbi_write_png("output.png", width, height, 3,
                        image.data(), width * 3)) {
        std::cerr << "Failed to write PNG\n";
        return 1;
    }

    std::cout << "Conversion successful!\n";
    return 0;
}