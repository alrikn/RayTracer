#include "AntiAliasing/Supersampling.hpp"
#include <cmath>
#include <random>

namespace RayTracer {

Math::Vector3d Supersampling::computePixel(
    double base_u,
    double base_v,
    unsigned int width,
    unsigned int height,
    const Camera &camera,
    std::function<Math::Vector3d(const Ray &, int)> traceRay
) const
{
    thread_local std::mt19937 rng(std::random_device{}());
    thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);

    int sqrtN = static_cast<int>(std::sqrt(static_cast<double>(_samples)));
    Math::Vector3d accumulated(0, 0, 0);
    double uStep = width > 1 ? 1.0 / static_cast<double>(width - 1) : 0.0;
    double vStep = height > 1 ? 1.0 / static_cast<double>(height - 1) : 0.0;

    for (int sy = 0; sy < sqrtN; sy++) {
        for (int sx = 0; sx < sqrtN; sx++) {
            double jx = (sx + dist(rng)) / sqrtN;
            double jy = (sy + dist(rng)) / sqrtN;
            double su = base_u + (jx - 0.5) * uStep;
            double sv = base_v + (jy - 0.5) * vStep;
            accumulated = accumulated + traceRay(camera.ray(su, sv), 0);        
        }
    }
    return accumulated / static_cast<double>(_samples);
}

}
