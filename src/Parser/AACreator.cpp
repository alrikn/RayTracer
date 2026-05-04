/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** AACreator
*/

#include "AntiAliasing/Supersampling.hpp"
#include "Parser.hpp"
#include <cmath>


std::unique_ptr<RayTracer::IAntiAliasing> Parser::createSupersampling(const libconfig::Setting &cfg)
{
    int samples = 4;
    if (cfg.exists("samples"))
        samples = static_cast<int>(parseDouble(cfg.lookup("samples")));
    if (samples < 1)
        throw std::runtime_error("[Antialiasing] samples must be >= 1, got " + std::to_string(samples));
    int sqrtN = static_cast<int>(std::sqrt(static_cast<double>(samples)));
    int rounded = sqrtN * sqrtN;
    if (rounded != samples) {
        std::cerr << "[Antialiasing] samples=" << samples
                  << " is not a perfect square. Rounded down to " << rounded << ".\n";
        samples = rounded;
    }
    return std::make_unique<RayTracer::Supersampling>(samples);
}
