/*
** Tests for RayTracer::Plane::hits()
**
** Plane defined by a point on the plane and a normal vector.
** Default: horizontal plane at y=0 with normal (0,1,0).
**
** Features tested:
**   - Standard ray-plane intersection
**   - Back-face: when ray comes from the "wrong" side, normal is flipped
**   - Parallel ray: no hit
**   - Chessboard pattern: produces hits (color-checking is impl-dependent)
*/
#include "helpers.hpp"
#include "Plane.hpp"

// ── Standard hits ─────────────────────────────────────────────────────────────

TEST_CASE("Plane: ray hits horizontal plane from above", "[plane]") {
    // Horizontal plane at y=0, normal (0,1,0).
    // Ray from (0,5,0) going straight down → hits at (0,0,0), distance=5.
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    auto hit = p.hits(make_ray(0,5,0, 0,-1,0));

    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->distance, 5.0);
    CHECK_APPROX(hit->point.x, 0.0);
    CHECK_APPROX(hit->point.y, 0.0);
    CHECK_APPROX(hit->point.z, 0.0);
}

TEST_CASE("Plane: normal points toward ray origin (front-face)", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    auto hit = p.hits(make_ray(0,5,0, 0,-1,0));

    REQUIRE(hit.has_value());
    // Front-face hit: normal should point upward (toward ray origin)
    CHECK(hit->normal.y > 0.0);
}

TEST_CASE("Plane: back-face hit flips normal toward ray origin", "[plane]") {
    // Ray from below (0,-5,0) going up hits the plane from the wrong side.
    // Normal should be flipped to point downward (toward the ray origin).
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    auto hit = p.hits(make_ray(0,-5,0, 0,1,0));

    REQUIRE(hit.has_value());
    CHECK(hit->normal.y < 0.0);
}

TEST_CASE("Plane: HitRecord color matches plane color", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    p.setColor(BLUE);
    auto hit = p.hits(make_ray(0,5,0, 0,-1,0));

    REQUIRE(hit.has_value());
    CHECK_COLOR(hit->color.z, 255.0);  // blue channel
}

// ── Misses ────────────────────────────────────────────────────────────────────

TEST_CASE("Plane: ray parallel to plane misses", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    // Ray traveling in XZ plane (direction has no Y component)
    auto hit = p.hits(make_ray(0,1,0, 1,0,0));
    CHECK(!hit.has_value());
}

TEST_CASE("Plane: ray pointing away from plane misses", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    // Ray starts above the plane and goes upward — t would be negative
    auto hit = p.hits(make_ray(0,5,0, 0,1,0));
    CHECK(!hit.has_value());
}

// ── Chessboard ────────────────────────────────────────────────────────────────

TEST_CASE("Plane: chessboard pattern enabled — hits() still returns a hit", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    p.setChessboardPattern(true);
    auto hit = p.hits(make_ray(0,5,0, 0,-1,0));
    // Chessboard should not prevent the hit from being registered
    CHECK(hit.has_value());
}

TEST_CASE("Plane: chessboard tiles at different positions have different colors", "[plane]") {
    RayTracer::Plane p{Math::Vector3d{0,1,0}, Math::Point3d{0,0,0}};
    p.setChessboardPattern(true);
    p.setColor(WHITE);

    // Hit at different XZ positions — should alternate colors
    auto h1 = p.hits(make_ray(0.5,  5, 0.5, 0,-1,0));
    auto h2 = p.hits(make_ray(1.5,  5, 0.5, 0,-1,0));

    REQUIRE(h1.has_value());
    REQUIRE(h2.has_value());

    // The two tiles should have different colors (one is the inverted color).
    bool same_color = (std::abs(h1->color.x - h2->color.x) <= COLOR_EPS &&
                       std::abs(h1->color.y - h2->color.y) <= COLOR_EPS &&
                       std::abs(h1->color.z - h2->color.z) <= COLOR_EPS);
    CHECK(!same_color);
}
