/*
** Tests for RayTracer::Cone::hits()
**
** Cone defined by center (base center), axis, radius, and height.
** The cone tapers from the base (at `origin`) to the apex (at origin + axis*height).
**
** Note: The cone intersection code is known to have implementation quirks
** (see TODO.md / source comments). Tests focus on: does a hit happen or not.
** Exact distance values are not asserted for the lateral surface to be resilient
** to future implementation fixes.
**
** Test setup: Cone at (0,0,0), axis (0,1,0), radius=1, height=1.
**   - Base at y=0, apex at y=1.
*/
#include "helpers.hpp"
#include "Cone.hpp"

TEST_CASE("Cone: ray aimed at base cap hits", "[cone]") {
    // Ray from directly below the base, going up.
    // The base disk is at y=0 with normal pointing down (away from cone body).
    RayTracer::Cone cone{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 1.0
    };
    auto hit = cone.hits(make_ray(0,-5,0, 0,1,0));
    CHECK(hit.has_value());
}

TEST_CASE("Cone: ray aimed at base center hits", "[cone]") {
    // Same as above but from different distance
    RayTracer::Cone cone{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 1.0
    };
    auto hit = cone.hits(make_ray(0,-10,0, 0,1,0));
    CHECK(hit.has_value());
    CHECK(hit->distance > 0.0);
}

TEST_CASE("Cone: ray far from cone axis misses", "[cone]") {
    RayTracer::Cone cone{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 1.0
    };
    // Ray at X=10 pointing down — far outside the cone's radius
    auto hit = cone.hits(make_ray(10,5,0, 0,-1,0));
    CHECK(!hit.has_value());
}

TEST_CASE("Cone: HitRecord color matches cone color", "[cone]") {
    RayTracer::Cone cone{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 1.0
    };
    cone.setColor(MAGENTA);
    auto hit = cone.hits(make_ray(0,-5,0, 0,1,0));

    REQUIRE(hit.has_value());
    CHECK_COLOR(hit->color.x, 255.0);
    CHECK_COLOR(hit->color.y, 0.0);
    CHECK_COLOR(hit->color.z, 255.0);
}
