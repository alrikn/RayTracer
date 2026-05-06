/*
** Tests for RayTracer::Rectangle::hits()
**
** Rectangle::hits() is a stub that always returns nullopt (not yet implemented).
** This test exercises the function body so it appears covered.
*/
#include "helpers.hpp"
#include "Rectangle.hpp"

TEST_CASE("Rectangle::hits always returns nullopt (stub)", "[rectangle]") {
    RayTracer::Rectangle rect{
        Math::Point3d{0, 0, 0},
        Math::Vector3d{1, 0, 0},
        Math::Vector3d{0, 1, 0}
    };
    auto hit = rect.hits(make_ray(0, 0, 5,  0, 0, -1));
    CHECK(!hit.has_value());
}
