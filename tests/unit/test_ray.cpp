/*
** Tests for RayTracer::Ray
**
** Ray is a simple value type: origin (Point3d), direction (Vector3d),
** and color_vector (Vector3d, defaults to white).
*/
#include "helpers.hpp"

// Note: Ray::Ray() default constructor is declared but never defined in the project
// (the codebase only uses the two-arg constructor). Tested via two-arg constructor.

TEST_CASE("Ray explicit constructor stores origin and direction", "[ray]") {
    RayTracer::Ray r{Math::Point3d{1,2,3}, Math::Vector3d{0,0,-1}};
    CHECK_APPROX(r.origin.x, 1.0);
    CHECK_APPROX(r.origin.y, 2.0);
    CHECK_APPROX(r.origin.z, 3.0);
    CHECK_APPROX(r.direction.x, 0.0);
    CHECK_APPROX(r.direction.y, 0.0);
    CHECK_APPROX(r.direction.z, -1.0);
}

TEST_CASE("Ray default color_vector is white (255, 255, 255)", "[ray]") {
    // Use two-arg constructor; color_vector defaults to white regardless.
    RayTracer::Ray r{Math::Point3d{0,0,0}, Math::Vector3d{0,0,-1}};
    CHECK_COLOR(r.color_vector.x, 255.0);
    CHECK_COLOR(r.color_vector.y, 255.0);
    CHECK_COLOR(r.color_vector.z, 255.0);
}

TEST_CASE("make_ray helper builds correct Ray", "[ray]") {
    auto r = make_ray(1,2,3, 0,0,-1);
    CHECK_APPROX(r.origin.x, 1.0);
    CHECK_APPROX(r.direction.z, -1.0);
}
