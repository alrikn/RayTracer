#if 0
/*
** Functional tests: reflection rendering end-to-end.
**
** Verifies that Scene::render() correctly includes reflected colors in the output
** image. Uses the same geometry as test_scene_reflections.cpp:
**
**   [red sphere]      camera        [black reflective sphere]
**   (0,0,+2)     <--  (0,0,0)  -->  (0,0,-2)  r=1
**
** All camera rays go in -Z direction and never hit the red sphere.
** Center pixels hit the black sphere. The reflected ray goes +Z and
** hits the red sphere, adding R≈127 to those pixels.
**
** Comparison:
**   max_depth=0 (no reflection): center pixels are black (R=0)
**   max_depth=4 (reflection on): center pixels have R≈127
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "AmbientLight.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <memory>

// WHY 5x5 AND NOT 10x10:
//
// A 10x10 render never sends an exact (0,0,-1) ray — the closest pixels
// are at u=4/9≈0.444 or u=5/9≈0.556, which give tilted directions like
// (-2/9, -1/9, -1). The reflected ray from a tilted hit goes diagonally
// and misses the small red sphere at (0,0,2) r=0.5.
//
// A 5x5 render has u = i/4 ∈ {0, 0.25, 0.5, 0.75, 1}. The center pixel
// (i=2, j=2) gives u=0.5, v=0.5 → direction exactly (0,0,-1) → normal
// (0,0,1) → reflected direction exactly (0,0,+1) → hits the red sphere.
static std::vector<std::array<int,3>> render_5x5(RayTracer::Scene& scene)
{
    scene.setwidth(5);
    scene.setheight(5);
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

static void add_objects(RayTracer::Scene& scene)
{
    auto mirror = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    mirror->setColor(BLACK);
    mirror->setReflectivity(0.5);
    scene.addShape(mirror);

    auto red_target = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,2}, 0.5);
    red_target->setColor(RED);
    red_target->setReflectivity(0.0);
    scene.addShape(red_target);

    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));
}

// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("Golden render: reflective sphere shows reflected color — max_depth=0 vs 4", "[functional][reflection]") {
    // Without reflection: black sphere in center → all pixels black.
    RayTracer::Scene scene_no_refl{1.0, 0};
    add_objects(scene_no_refl);
    auto pixels_no_refl = render_5x5(scene_no_refl);

    int max_red_no_refl = 0;
    for (auto& p : pixels_no_refl) max_red_no_refl = std::max(max_red_no_refl, p[0]);
    CHECK(max_red_no_refl == 0);  // black sphere, no reflection → zero red

    // With reflection: center pixel (2,2) reflects exactly to red sphere → R=127.
    RayTracer::Scene scene_refl{1.0, 4};
    add_objects(scene_refl);
    auto pixels_refl = render_5x5(scene_refl);

    int max_red_refl = 0;
    for (auto& p : pixels_refl) max_red_refl = std::max(max_red_refl, p[0]);
    CHECK(max_red_refl > 50);  // reflected red sphere visible

    // Reflection adds color that was not there before
    CHECK(max_red_refl > max_red_no_refl);
}

#endif
