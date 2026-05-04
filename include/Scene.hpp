/*
** EPITECH PROJECT, 2026
** temp_raytracer
** File description:
** Scene
*/

#ifndef INCLUDED_Scene_HPP
    #define INCLUDED_Scene_HPP

#include "ILight.hpp"
#include "IShape.hpp"
#include "Camera.hpp"

#include <vector>
#include <array>
#include <memory>
#include <string>
#include <iosfwd>
#include <atomic>
#include <mutex>
#include <cstdint>

namespace RayTracer {

class Scene
{
    private:
        std::vector<std::shared_ptr<IShape>> _objects;
        std::vector<std::shared_ptr<ILight>> _lights;

        Camera _camera;
        unsigned int width = 4000; //width of the output image
        unsigned int height = 2000; //height of the output image

        double brightness = 1.0; //global brightness. max is one, min is 0. it just touches final color values, doesn't touch lights.

        const int max_depth = 4; //max recursion
        const double epsilon = 1e-4; //small value to prevent selfintersection

        /*helper funcs*/
        Math::Vector3d average_light(std::vector<Math::Vector3d> light_contributions) const;
        double clamp_color(double x) const;
        void write_color(const std::array<uint8_t, 3> &color, std::string &output) const;
        void renderChunk(std::vector<std::vector<std::array<uint8_t, 3>>> &pixels, std::atomic<int> &columns_rendered, std::mutex &cerr_mutex, int start_col, int end_col) const;
        std::string serializeBuffer(const std::vector<std::vector<std::array<uint8_t, 3>>> &pixels) const;
    public:
        Scene(double brightness = 0.9, int max_depth = 4) : brightness(brightness), max_depth(max_depth) {}
        ~Scene() = default;

        void addShape(const std::shared_ptr<IShape> &object);
        void addLight(const std::shared_ptr<ILight> &light);
        Math::Vector3d traceRay(const Ray &ray, int depth) const; //overloaded func
        void render(std::ostream &output) const;

        void setCamera(const Camera &camera) { _camera = camera; }
        void setwidth(unsigned int w) { width = w; }
        void setheight(unsigned int h) { height = h; }

        unsigned int getWidth() const { return width; }
        unsigned int getHeight() const { return height; }
};
}

#endif
