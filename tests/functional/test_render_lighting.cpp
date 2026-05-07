#if 0
/*
** Functional tests: lighting render behavior.
**
** Covers: directional light producing top-brighter-than-bottom gradient.
** Add more tests here when you implement specular renders, multiple lights, etc.
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "DirectionalLight.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <memory>

static std::vector<std::array<int,3>> render_10x10(RayTracer::Scene& scene)
{
    scene.setwidth(10);
    scene.setheight(10);
    std::ostringstream oss;
    scene.render(oss);
    std::string ppm = oss.str();
    std::istringstream ss(ppm);
    std::string magic;
    int w, h, maxval;
    ss >> magic >> w >> h >> maxval;
    std::vector<std::array<int,3>> pixels;
    pixels.reserve(w * h);
    for (int i = 0; i < w * h; ++i) {
        int r, g, b;
        ss >> r >> g >> b;
        pixels.push_back({r, g, b});
    }
    return pixels;
}

static std::array<int,3> pixel_at(const std::vector<std::array<int,3>>& px,
                                  int col, int row, int width = 10)
{
    return px[row * width + col];
}

// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Golden render: directional light from above — top rows brighter than bottom rows", "[functional][lighting]") {
    // White sphere at (0,0,-1) r=0.8, directional light from above.
    // The top of the sphere (normal facing light) gets full lambert=1.
    // The bottom (normal facing away) gets lambert=0 → black.
    // So the top rows of the image should be brighter overall.
    RayTracer::Scene scene{1.0, 4};
    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-1}, 0.8);
    sphere->setColor(WHITE);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}
    ));

    auto pixels = render_10x10(scene);

    int top_brightness = 0, bot_brightness = 0;
    for (int col = 0; col < 10; ++col) {
        auto top = pixel_at(pixels, col, 0);
        auto bot = pixel_at(pixels, col, 9);
        top_brightness += top[0] + top[1] + top[2];
        bot_brightness += bot[0] + bot[1] + bot[2];
    }
    CHECK(top_brightness >= bot_brightness);
}

#endif
