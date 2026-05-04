#ifndef INCLUDED_IAntiAliasing_HPP
#define INCLUDED_IAntiAliasing_HPP

#include "Camera.hpp"
#include "Ray.hpp"
#include "Math/Vector3d.hpp"
#include <functional>

namespace RayTracer {

class IAntiAliasing {
public:
    virtual ~IAntiAliasing() = default;
    virtual Math::Vector3d computePixel(
        double base_u,
        double base_v,
        unsigned int width,
        unsigned int height,
        const Camera &camera,
        std::function<Math::Vector3d(const Ray &, int)> traceRay
    ) const = 0;
};

}
#endif