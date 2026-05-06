/*
** Functional tests: basic shape rendering.
**
** Covers: empty scene, sphere, plane, cylinder, cone — all rendered to a
** 10×10 image. Each test checks:
**   - max channel > 200  (the shape is actually rendered somewhere)
**   - corners are black  (the shape does not fill the entire screen)
**
** These are the baseline tests — if these fail, everything else is suspect.
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "AmbientLight.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <memory>

// Shared PPM helpers — duplicated across functional test files intentionally.
// Each file is self-contained so it can be run and read independently.

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

TEST_CASE("Golden render: empty scene produces all-black image", "[functional][shapes]") {
    RayTracer::Scene scene{1.0, 4};
    auto pixels = render_10x10(scene);

    int total = 0;
    for (auto& p : pixels) total += p[0] + p[1] + p[2];
    CHECK(total == 0);
}

TEST_CASE("Golden render: red sphere with ambient — center region is red, corners black", "[functional][shapes]") {
    // Red sphere at (0,0,-2) r=1 is large enough to cover the center pixels of
    // a 10×10 image. White ambient at intensity=1, brightness=1.
    RayTracer::Scene scene{1.0, 4};
    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    sphere->setColor(RED);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto pixels = render_10x10(scene);

    int max_red = 0;
    for (auto& p : pixels) max_red = std::max(max_red, p[0]);
    CHECK(max_red > 200);

    // Far corners always miss the sphere
    CHECK(pixel_at(pixels, 0, 0)[0] == 0);
    CHECK(pixel_at(pixels, 0, 0)[1] == 0);
    CHECK(pixel_at(pixels, 0, 0)[2] == 0);
    CHECK(pixel_at(pixels, 9, 9)[0] == 0);
    CHECK(pixel_at(pixels, 9, 9)[1] == 0);
    CHECK(pixel_at(pixels, 9, 9)[2] == 0);
}

TEST_CASE("Golden render: plane — bottom half of image is green", "[functional][shapes]") {
    // Horizontal green plane at y=-1, normal (0,1,0).
    // Camera rays with direction.y < 0 (v < 0.5, bottom half of screen) hit the plane.
    // Rays with direction.y >= 0 (top half) miss or run parallel.
    // Geometry: t = -1 / direction.y, positive only when direction.y < 0.
    RayTracer::Scene scene{1.0, 4};
    auto plane = std::make_shared<RayTracer::Plane>(
        Math::Vector3d{0,1,0}, Math::Point3d{0,-1,0}
    );
    plane->setColor(GREEN);
    plane->setReflectivity(0.0);
    scene.addShape(plane);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto pixels = render_10x10(scene);

    int max_green = 0;
    for (auto& p : pixels) max_green = std::max(max_green, p[1]);
    CHECK(max_green > 200);

    // Top-left corner: direction.y = -1+2*1 = 1 > 0 → misses the plane
    CHECK(pixel_at(pixels, 0, 0)[1] == 0);
}

TEST_CASE("Golden render: cylinder — center pixels are cyan", "[functional][shapes]") {
    // Vertical cyan cylinder at (0,0,-3), axis Y, r=1.0, h=3.
    // Center camera rays hit the side surface.
    // Geometry verified: disc = 36 - 4*((-2+4u)²+1)*(9-1).
    // For u≈0.444 or 0.556 and v≈0.556: disc > 0, height ∈ [0,3]. Hit.
    RayTracer::Scene scene{1.0, 4};
    auto cyl = std::make_shared<RayTracer::Cylinder>(
        Math::Point3d{0,0,-3}, Math::Vector3d{0,1,0}, 1.0, 3.0
    );
    cyl->setColor(CYAN);
    cyl->setReflectivity(0.0);
    scene.addShape(cyl);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto pixels = render_10x10(scene);

    // CYAN = (0,255,255) — check blue channel
    int max_blue = 0;
    for (auto& p : pixels) max_blue = std::max(max_blue, p[2]);
    CHECK(max_blue > 200);

    // Top-left corner is far off-axis: disc < 0, no hit
    CHECK(pixel_at(pixels, 0, 0)[2] == 0);
    CHECK(pixel_at(pixels, 9, 9)[2] == 0);
}

TEST_CASE("Golden render: cone base — center pixels are yellow", "[functional][shapes]") {
    // Yellow cone at (0,0,-3), axis (0,0,-1) (pointing away from camera), r=1.5, h=2.
    // Base disk at z=-3 faces the camera with normal (0,0,+1).
    // Center camera ray hits base at t=3, offset (x,y) from axis ≤ r=1.5. Hit.
    // Verified: for u∈{4/9,5/9}, v∈{4/9,5/9}: offset = sqrt(5)/3 ≈ 0.745 < 1.5.
    RayTracer::Scene scene{1.0, 4};
    auto cone = std::make_shared<RayTracer::Cone>(
        Math::Point3d{0,0,-3}, Math::Vector3d{0,0,-1}, 1.5, 2.0
    );
    cone->setColor(YELLOW);
    cone->setReflectivity(0.0);
    scene.addShape(cone);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto pixels = render_10x10(scene);

    // YELLOW = (255,255,0) — check red channel
    int max_red = 0;
    for (auto& p : pixels) max_red = std::max(max_red, p[0]);
    CHECK(max_red > 200);

    // Corners: offset from axis ≈ 6.7 >> r=1.5, definitely misses
    CHECK(pixel_at(pixels, 0, 0)[0] == 0);
    CHECK(pixel_at(pixels, 9, 9)[0] == 0);
}

TEST_CASE("Functional: render with width=0 produces no output", "[functional][threadmgmt]") {
    // Covers ThreadManagement.cpp line 19: if(width==0||height==0) return.
    RayTracer::Scene scene{1.0, 4};
    scene.setwidth(0);
    scene.setheight(10);

    std::ostringstream oss;
    scene.render(oss);

    CHECK(oss.str().empty());
}

TEST_CASE("Functional: render with height=0 produces no output", "[functional][threadmgmt]") {
    // Covers ThreadManagement.cpp line 19 via the height==0 path.
    RayTracer::Scene scene{1.0, 4};
    scene.setwidth(10);
    scene.setheight(0);

    std::ostringstream oss;
    scene.render(oss);

    CHECK(oss.str().empty());
}
