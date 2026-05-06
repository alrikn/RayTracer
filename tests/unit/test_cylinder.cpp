/*
** Tests for RayTracer::Cylinder::hits()
**
** Cylinder defined by center (base center), axis, radius, and height.
** has three zones to hit: lateral surface, front cap (base), back cap (top).
**
** Test setup: Cylinder at (0,0,0), axis (0,1,0), radius=1, height=3.
**   - Base at y=0, top at y=3.
**   - Lateral surface: distance from Y-axis = 1 at any y ∈ [0,3].
*/
#include "helpers.hpp"
#include "Cylinder.hpp"

// ── Lateral surface hits ──────────────────────────────────────────────────────

TEST_CASE("Cylinder: ray hits lateral surface from the side", "[cylinder]") {
    // Ray from (0,0,5) toward -Z hits the front of the cylinder at z=1 (the
    // closest point on the cylinder's circular cross-section at y=0).
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    auto hit = cyl.hits(make_ray(0,0,5, 0,0,-1));

    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->distance, 4.0);
    CHECK_APPROX(hit->point.z, 1.0);
    // Normal on lateral surface must be perpendicular to the cylinder axis
    CHECK_APPROX(hit->normal.dot(Math::Vector3d{0,1,0}), 0.0);
}

TEST_CASE("Cylinder: lateral surface normal points radially outward", "[cylinder]") {
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    auto hit = cyl.hits(make_ray(0,0,5, 0,0,-1));

    REQUIRE(hit.has_value());
    // The hit is at z=1, so the normal should point in the +Z direction
    CHECK(hit->normal.z > 0.0);
}

// ── Cap hits ──────────────────────────────────────────────────────────────────

TEST_CASE("Cylinder: ray hits top cap from above", "[cylinder]") {
    // Ray from (0,5,0) going straight down hits the top cap at y=3. distance=2.
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    auto hit = cyl.hits(make_ray(0,5,0, 0,-1,0));

    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->distance, 2.0);
    CHECK_APPROX(hit->point.y, 3.0);
}

TEST_CASE("Cylinder: ray hits bottom cap from below", "[cylinder]") {
    // Ray from (0,-5,0) going up hits the base cap at y=0. distance=5.
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    auto hit = cyl.hits(make_ray(0,-5,0, 0,1,0));

    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->distance, 5.0);
    CHECK_APPROX(hit->point.y, 0.0);
}

// ── Misses ────────────────────────────────────────────────────────────────────

TEST_CASE("Cylinder: ray misses outside radius", "[cylinder]") {
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    // Ray from (5,0,0) going in -Z — completely beside the cylinder
    auto hit = cyl.hits(make_ray(5,0,0, 0,0,-1));
    CHECK(!hit.has_value());
}

TEST_CASE("Cylinder: horizontal ray above height misses", "[cylinder]") {
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    // Ray at y=10 (above height=3), going in +X direction.
    // It would hit an infinite cylinder at that height, but height check filters it.
    auto hit = cyl.hits(make_ray(0,10,0, 1,0,0));
    CHECK(!hit.has_value());
}

TEST_CASE("Cylinder: HitRecord color matches cylinder color", "[cylinder]") {
    RayTracer::Cylinder cyl{
        Math::Point3d{0,0,0},
        Math::Vector3d{0,1,0},
        1.0, 3.0
    };
    cyl.setColor(CYAN);
    auto hit = cyl.hits(make_ray(0,0,5, 0,0,-1));

    REQUIRE(hit.has_value());
    CHECK_COLOR(hit->color.x, 0.0);
    CHECK_COLOR(hit->color.y, 255.0);
    CHECK_COLOR(hit->color.z, 255.0);
}
