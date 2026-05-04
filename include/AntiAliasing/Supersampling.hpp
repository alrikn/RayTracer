#ifndef INCLUDED_Supersampling_HPP
#define INCLUDED_Supersampling_HPP

#include "AntiAliasing/IAntiAliasing.hpp"

namespace RayTracer {

class Supersampling : public IAntiAliasing {
    int _samples;
public:
    explicit Supersampling(int samples) : _samples(samples) {}
    Math::Vector3d computePixel(
        double base_u,
        double base_v,
        unsigned int width,
        unsigned int height,
        const Camera &camera,
        std::function<Math::Vector3d(const Ray &, int)> traceRay
    ) const override;
};

}
#endif
