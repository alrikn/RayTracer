/*
** Functional tests: anti-aliasing techniques.
**
** Covers: Supersampling (4 samples, 9 samples), AA vs no-AA diff.
**
** HOW TO ADD A TEST FOR A NEW AA TECHNIQUE:
**   1. Add #include for your new AA header at the top.
**   2. Copy one of the Supersampling tests below.
**   3. Replace make_unique<Supersampling>(N) with your new technique.
**   4. Keep the same assertions — they test the IAntiAliasing interface,
**      not the specific algorithm. Add algorithm-specific checks if needed.
**
** WHY NO EXACT PIXEL VALUES:
**   Supersampling uses random jitter (thread_local RNG), so values differ
**   each run. Statistical checks (max channel, corner black, diff count) are
**   reliable because they describe properties that hold for any valid AA output.
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "AmbientLight.hpp"
#include "AntiAliasing/Supersampling.hpp"
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

// Standard scene used by all AA tests — keep it identical so comparisons are fair.
static void setup_aa_scene(RayTracer::Scene& scene)
{
    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    sphere->setColor(RED);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));
}

// ─────────────────────────────────────────────────────────────────────────────

TEST_CASE("AA: Supersampling(4) — sphere still renders correctly", "[functional][aa]") {
    // Smoke test: enabling 4-sample supersampling must not crash and must
    // still produce a visible red sphere with black corners.
    RayTracer::Scene scene{1.0, 4};
    setup_aa_scene(scene);
    scene.setAA(std::make_unique<RayTracer::Supersampling>(4));

    auto pixels = render_10x10(scene);

    int max_red = 0;
    for (auto& p : pixels) max_red = std::max(max_red, p[0]);
    CHECK(max_red > 200);

    CHECK(pixel_at(pixels, 0, 0)[0] == 0);
    CHECK(pixel_at(pixels, 9, 9)[0] == 0);
}

TEST_CASE("AA: Supersampling(9) — 3×3 grid variant still renders", "[functional][aa]") {
    // 9 samples = 3×3 grid. Same assertions as 4-sample to verify a
    // different sample count also works through the IAntiAliasing interface.
    RayTracer::Scene scene{1.0, 4};
    setup_aa_scene(scene);
    scene.setAA(std::make_unique<RayTracer::Supersampling>(9));

    auto pixels = render_10x10(scene);

    int max_red = 0;
    for (auto& p : pixels) max_red = std::max(max_red, p[0]);
    CHECK(max_red > 200);
    CHECK(pixel_at(pixels, 0, 0)[0] == 0);
}

TEST_CASE("AA: Supersampling output differs from no-AA output", "[functional][aa]") {
    // Edge pixels of the sphere are blended by supersampling but binary (0 or
    // full color) without AA. The two renders must differ somewhere.
    // This confirms setAA() actually changes the rendering path.
    RayTracer::Scene scene_no_aa{1.0, 4};
    setup_aa_scene(scene_no_aa);
    auto pixels_no_aa = render_10x10(scene_no_aa);

    RayTracer::Scene scene_aa{1.0, 4};
    setup_aa_scene(scene_aa);
    scene_aa.setAA(std::make_unique<RayTracer::Supersampling>(4));
    auto pixels_aa = render_10x10(scene_aa);

    int differing_pixels = 0;
    for (int i = 0; i < 100; ++i) {
        if (pixels_no_aa[i][0] != pixels_aa[i][0] ||
            pixels_no_aa[i][1] != pixels_aa[i][1] ||
            pixels_no_aa[i][2] != pixels_aa[i][2]) {
            ++differing_pixels;
        }
    }
    CHECK(differing_pixels > 0);
}
