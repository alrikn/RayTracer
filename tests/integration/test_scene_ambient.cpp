/*
** Integration tests: ambient lighting and basic Scene::traceRay() behavior.
**
** Covers: empty scene, sphere + ambient light, ray misses, brightness multiplier.
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "AmbientLight.hpp"

TEST_CASE("Scene::traceRay: empty scene returns black", "[scene][ambient]") {
    RayTracer::Scene scene{1.0, 4};
    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);
    CHECK_COLOR_VEC3(color, 0.0, 0.0, 0.0);
}

TEST_CASE("Scene::traceRay: ray that misses all objects returns black", "[scene][ambient]") {
    RayTracer::Scene scene{1.0, 4};
    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    sphere->setColor(RED);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    // Ray goes upward — misses the sphere which is in front (at -Z)
    auto color = scene.traceRay(make_ray(0,0,0, 0,1,0), 0);
    CHECK_COLOR_VEC3(color, 0.0, 0.0, 0.0);
}

TEST_CASE("Scene::traceRay: red sphere with white ambient gives red", "[scene][ambient]") {
    // brightness=1.0, ambient intensity=1.0, reflectivity=0:
    // result = (255,255,255) * 1.0 * (255,0,0)/255 * brightness = (255,0,0).
    RayTracer::Scene scene{1.0, 4};

    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    sphere->setColor(RED);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);
    CHECK_COLOR(color.x, 255.0);
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
}

TEST_CASE("Scene::traceRay: brightness 0.5 halves the output color", "[scene][ambient]") {
    RayTracer::Scene scene{0.5, 4};

    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 1.0);
    sphere->setColor(RED);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::AmbientLight>(1.0));

    auto color = scene.traceRay(make_ray(0,0,0, 0,0,-1), 0);
    CHECK_COLOR(color.x, 127.5);
    CHECK_COLOR(color.y, 0.0);
    CHECK_COLOR(color.z, 0.0);
}
