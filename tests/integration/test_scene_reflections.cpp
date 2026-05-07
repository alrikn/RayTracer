/*
** Integration tests: reflection recursion in Scene::traceRay().
**
** Geometry used in all tests:
**
**   [red sphere]      camera        [black reflective sphere]
**   (0,0, +2)    <--  (0,0,0)  -->  (0,0,-2)   r=1
**
**   Primary ray goes in -Z direction.
**   It hits the black sphere (at -Z) and MISSES the red sphere (at +Z,
**   behind the camera — all k values are negative for the primary ray).
**   The reflected ray bounces off the black sphere's front face, goes in +Z,
**   and hits the red sphere.
**
**   Direct light on black sphere = 0 (black * ambient = 0).
**   Reflected light = red = (255,0,0).
**   Final = 0 + reflectivity * (255,0,0).
**
** This lets us test reflection independently of direct lighting:
**   - If reflections are disabled → color is (0,0,0)
**   - If reflections are enabled  → color has R > 0
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "AmbientLight.hpp"

// ── Helpers ───────────────────────────────────────────────────────────────────

// Both reflection tests use the same scene contents — only max_depth changes.
static void add_reflection_objects(RayTracer::Scene& scene)
{
    auto mirror = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    mirror->setColor(BLACK);          // black → direct ambient contribution = 0
    mirror->setReflectivity(0.5);
    scene.addShape(mirror);

    auto red_target = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,2}, 0.5);
    red_target->setColor(RED);
    red_target->setReflectivity(0.0);
    scene.addShape(red_target);

    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));
}

// ── Tests ─────────────────────────────────────────────────────────────────────

TEST_CASE("Scene::traceRay: max_depth=0 disables reflection", "[scene][reflection]") {
    // depth(0) >= max_depth(0) → reflection branch is skipped entirely.
    // Black sphere with reflectivity=0.5 returns only direct light = (0,0,0).
    RayTracer::Scene scene{1.0, 0};
    add_reflection_objects(scene);

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);

    CHECK_COLOR(color.x, 0.0);
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
}

TEST_CASE("Scene::traceRay: reflective sphere picks up color from reflected target", "[scene][reflection]") {
    // depth(0) < max_depth(4) → reflection IS computed.
    // Reflected ray hits red sphere → R channel becomes 127.5 (0.5 * 255).
    RayTracer::Scene scene{1.0, 4};
    add_reflection_objects(scene);

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);

    CHECK(color.x > 0.0);     // red from reflected target is visible
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
    CHECK_COLOR(color.x, 127.5);  // 0.5 * 255 = 127.5
}

TEST_CASE("Scene::traceRay: reflectivity=0 gives same result as max_depth=0", "[scene][reflection]") {
    // Even with max_depth=4, reflectivity=0 skips the reflection branch.
    RayTracer::Scene scene{1.0, 4};

    auto mirror = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    mirror->setColor(BLACK);
    mirror->setReflectivity(0.0);  // no reflection
    scene.addShape(mirror);

    auto red_target = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,2}, 0.5);
    red_target->setColor(RED);
    red_target->setReflectivity(0.0);
    scene.addShape(red_target);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);

    CHECK_COLOR(color.x, 0.0);  // no reflection → black sphere stays black
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
}

TEST_CASE("Scene::traceRay: reflectivity=1.0 gives full reflected color", "[scene][reflection]") {
    // Perfect mirror (reflectivity=1.0): reflected light is added at full strength.
    // final = 0 + 1.0 * (255,0,0) = (255,0,0).
    RayTracer::Scene scene{1.0, 4};

    auto mirror = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    mirror->setColor(BLACK);
    mirror->setReflectivity(1.0);
    scene.addShape(mirror);

    auto red_target = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,2}, 0.5);
    red_target->setColor(RED);
    red_target->setReflectivity(0.0);
    scene.addShape(red_target);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);

    CHECK_COLOR(color.x, 255.0);
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
}

TEST_CASE("Scene::traceRay: recursive reflections terminate without crash", "[scene][reflection]") {
    // Two facing spheres both with high reflectivity — infinite bounces without max_depth.
    // max_depth=4 must terminate the recursion cleanly (no stack overflow, no hang).
    RayTracer::Scene scene{1.0, 4};

    auto sphere_a = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 0.5);
    sphere_a->setColor(WHITE);
    sphere_a->setReflectivity(0.9);
    scene.addShape(sphere_a);

    auto sphere_b = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,2}, 0.5);
    sphere_b->setColor(WHITE);
    sphere_b->setReflectivity(0.9);
    scene.addShape(sphere_b);

    scene.addLight(std::make_shared<RayTracer::AmbientLight>(0.5));

    // Just must not crash or infinite-loop — any color is acceptable
    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);
    CHECK(color.x >= 0.0);
    CHECK(color.x <= 255.0);
}

