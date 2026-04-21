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
        Math::Vector3d colorFromShape(Color color) const;
        std::vector<std::shared_ptr<IShape>> _objects;
        //std::vector<std::shared_ptr<ILight>> _lights; i am usnure if there needs to be more than one lighting at the same time. for now, with just a sngle type it should be more than enough
        std::shared_ptr<ILight> _light;
    public:
        Matrix() = default;
        ~Matrix() = default;

        void addObject(const std::shared_ptr<IShape> &object);
        void addLight(const std::shared_ptr<ILight> &light);
        Math::Vector3d traceRay(const Ray &ray) const;
        void render(const Camera &camera, int width, int height, std::ostream &output) const;
};
}

#endif
