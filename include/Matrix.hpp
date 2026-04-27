/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Matrix
*/

#ifndef INCLUDED_MATRIX_HPP
    #define INCLUDED_MATRIX_HPP

#include "ILight.hpp"
#include "IShape.hpp"
#include "Camera.hpp"

#include <vector>
#include <memory>

namespace RayTracer {

class Matrix
{
    private:
        std::vector<std::shared_ptr<IShape>> _objects;
        std::vector<std::shared_ptr<ILight>> _lights;
        double brightness = 1.0; //global brightness. max is one, min is 0. it just touches final color values, doesn't touch lights.

        const int max_depth = 4; //max recursion
        const double epsilon = 1e-4; //small value to prevent selfintersection
    public:
        Matrix(double brightness = 0.9) : brightness(brightness) {}
        ~Matrix() = default;

        void addObject(const std::shared_ptr<IShape> &object);
        void addLight(const std::shared_ptr<ILight> &light);
        Math::Vector3d traceRay(const Ray &ray) const;
        Math::Vector3d traceRay(const Ray &ray, int depth) const; //overloaded func
        void render(const Camera &camera, int width, int height, std::ostream &output) const;
};
}

#endif
