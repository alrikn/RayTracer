/*
** Tests for RayTracer::AmbientLight::intensity()
**
** Formula: result = light_color * light_intensity * (hit.color / 255.0)
**
** IMPORTANT IMPLEMENTATION NOTE:
** AmbientLight::intensity() returns black if objects.empty() (early-return guard
** added to silence an unused-parameter warning). Always pass at least one object.
** Use `dummy_objects()` from this file for tests that don't need specific blockers.
**
** HOW TO ADD A NEW LIGHT TEST:
**   1. Build the light:        AmbientLight light{intensity, color};
**   2. Build a fake HitRecord: auto hit = make_hit(px,py,pz, nx,ny,nz, cr,cg,cb);
**   3. Call:                   auto result = light.intensity(hit, dummy_objects());
**   4. Check result with:      CHECK_COLOR_VEC3(result, r, g, b);
*/
#include "helpers.hpp"
#include "AmbientLight.hpp"
#include "Sphere.hpp"
#include <vector>
#include <memory>

// A non-empty objects vector with one sphere that is far away and never blocks anything.
// Needed because AmbientLight returns black on empty vectors (implementation guard).
static std::vector<std::shared_ptr<RayTracer::IShape>> dummy_objects() {
    return {std::make_shared<RayTracer::Sphere>(Math::Point3d{0,1000,0}, 0.1)};
}

TEST_CASE("AmbientLight: white light on red object gives red", "[ambient_light]") {
    // (255,255,255) * 1.0 * (255,0,0)/255 = (255, 0, 0)
    RayTracer::AmbientLight light{1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto result = light.intensity(hit, dummy_objects());
    CHECK_COLOR_VEC3(result, 255.0, 0.0, 0.0);
}

TEST_CASE("AmbientLight: half intensity on white object gives gray", "[ambient_light]") {
    // (255,255,255) * 0.5 * (255,255,255)/255 = (127.5, 127.5, 127.5)
    RayTracer::AmbientLight light{0.5, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,255,255);
    auto result = light.intensity(hit, dummy_objects());
    CHECK_COLOR_VEC3(result, 127.5, 127.5, 127.5);
}

TEST_CASE("AmbientLight: colored light on white object gives light color", "[ambient_light]") {
    // (255,0,0) * 1.0 * (255,255,255)/255 = (255, 0, 0)
    RayTracer::AmbientLight light{1.0, Math::Vector3d{255,0,0}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,255,255);
    auto result = light.intensity(hit, dummy_objects());
    CHECK_COLOR_VEC3(result, 255.0, 0.0, 0.0);
}

TEST_CASE("AmbientLight: zero intensity produces black", "[ambient_light]") {
    // _light_intensity <= 0 also triggers the early return (returns black)
    RayTracer::AmbientLight light{0.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,255,255);
    auto result = light.intensity(hit, dummy_objects());
    CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
}

TEST_CASE("AmbientLight: ignores blocker — result same with or without blocking sphere", "[ambient_light]") {
    // Ambient light casts no shadow rays — a sphere directly above the hit point
    // should NOT affect the result.
    RayTracer::AmbientLight light{1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);

    // Sphere far from path — just satisfies the non-empty requirement
    auto objs_no_blocker = dummy_objects();

    // Sphere directly above — would block a directional light, but not ambient
    auto blocker = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0.5,0}, 0.1);
    std::vector<std::shared_ptr<RayTracer::IShape>> objs_with_blocker = {blocker};

    auto r1 = light.intensity(hit, objs_no_blocker);
    auto r2 = light.intensity(hit, objs_with_blocker);

    CHECK_COLOR(r1.x, r2.x);
    CHECK_COLOR(r1.y, r2.y);
    CHECK_COLOR(r1.z, r2.z);
}
