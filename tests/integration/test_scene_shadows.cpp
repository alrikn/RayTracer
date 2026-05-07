/*
** Integration tests: directional lighting and shadow casting in Scene::traceRay().
**
** Covers: directional light illumination, shadow blocking via a plane+blocker setup.
*/
#include "helpers.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "DirectionalLight.hpp"
#include <memory>

TEST_CASE("Scene::traceRay: directional light illuminates unshadowed surface", "[scene][shadow]") {
    // Blue sphere at (0,0,-2) r=0.5. Light from above (direction (0,-1,0)).
    // Ray from (0,5,-2) going down hits the TOP of the sphere: normal=(0,1,0),
    // lambert = (0,1,0)·(0,1,0) = 1.0 → full blue contribution.
    RayTracer::Scene scene{1.0, 4};

    auto sphere = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,0,-2}, 0.5);
    sphere->setColor(BLUE);
    sphere->setReflectivity(0.0);
    scene.addShape(sphere);
    scene.addLight(std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}
    ));

    auto color = scene.traceRay(make_ray(0,5,-2, 0,-1,0), 0);
    CHECK(color.z > 0.0);
}

TEST_CASE("Scene::traceRay: shadow — plane with blocker casts darker region", "[scene][shadow]") {
    // A horizontal white plane at y=0. DirectionalLight from above (dir (0,-1,0)).
    // A blocker sphere at (3,2,0) r=0.8 sits above position (3,0,0) on the plane.
    //
    // Geometry that avoids hitting the blocker with the primary ray:
    //   lit  ray: (0,5,0) → (0,-1,0)   hits plane at (0,0,0)   — no blocker above
    //   shad ray: (0,5,0) → diag       hits plane at (3,0,0)   — blocker IS above
    //
    // The diagonal primary ray for shad misses the blocker (discriminant < 0).
    // The shadow ray from (3,0,0) going straight up hits the blocker → black.
    RayTracer::Scene scene{1.0, 4};

    auto plane = std::make_shared<RayTracer::Plane>(
        Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}
    );
    plane->setColor(WHITE);
    plane->setReflectivity(0.0);

    auto blocker = std::make_shared<RayTracer::Sphere>(Math::Point3d{3,2,0}, 0.8);
    blocker->setColor(RED);
    blocker->setReflectivity(0.0);

    scene.addShape(plane);
    scene.addShape(blocker);
    scene.addLight(std::make_shared<RayTracer::DirectionalLight>(
        Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}
    ));

    // lit: straight down, hits (0,0,0) — no blocker anywhere near this path
    auto lit = scene.traceRay(make_ray(0,5,0, 0,-1,0), 0);

    // shad: diagonal ray hits plane at (3,0,0) — blocker is directly above
    Math::Vector3d diag{3,-5,0};
    diag.normalizeSelf();
    auto shad = scene.traceRay(RayTracer::Ray{Math::Point3d{0,5,0}, diag}, 0);

    CHECK(lit.x > shad.x);
}
