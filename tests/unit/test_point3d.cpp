/*
** Tests for Math::Point3d
**
** Point3d and Vector3d are semantically different:
**   Point + Vector → Point  (translation)
**   Point - Point  → Vector (displacement)
**   Point + Point  → Vector (sum of coordinates, rarely needed but defined)
** These tests verify that distinction is enforced by the operators.
*/
#include "helpers.hpp"
#include "Math/Point3d.hpp"
#include "Math/Vector3d.hpp"

TEST_CASE("Point3d default constructor initializes to zero", "[point3d]") {
    Math::Point3d p;
    CHECK_APPROX(p.x, 0.0);
    CHECK_APPROX(p.y, 0.0);
    CHECK_APPROX(p.z, 0.0);
}

TEST_CASE("Point3d explicit constructor stores values", "[point3d]") {
    Math::Point3d p{1.0, 2.5, -3.0};
    CHECK_APPROX(p.x, 1.0);
    CHECK_APPROX(p.y, 2.5);
    CHECK_APPROX(p.z, -3.0);
}

TEST_CASE("Point3d + Vector3d translates point", "[point3d]") {
    Math::Point3d p{1,2,3};
    Math::Vector3d v{10,20,30};
    Math::Point3d result = p + v;
    CHECK_APPROX(result.x, 11.0);
    CHECK_APPROX(result.y, 22.0);
    CHECK_APPROX(result.z, 33.0);
}

TEST_CASE("Point3d - Vector3d translates point backwards", "[point3d]") {
    Math::Point3d p{5,5,5};
    Math::Vector3d v{2,2,2};
    Math::Point3d result = p - v;
    CHECK_APPROX(result.x, 3.0);
    CHECK_APPROX(result.y, 3.0);
    CHECK_APPROX(result.z, 3.0);
}

TEST_CASE("Point3d - Point3d gives displacement vector", "[point3d]") {
    Math::Point3d a{5,5,5};
    Math::Point3d b{2,2,2};
    Math::Vector3d result = a - b;
    CHECK_APPROX(result.x, 3.0);
    CHECK_APPROX(result.y, 3.0);
    CHECK_APPROX(result.z, 3.0);
}

TEST_CASE("Point3d + Point3d gives coordinate sum as vector", "[point3d]") {
    Math::Point3d a{1,2,3};
    Math::Point3d b{4,5,6};
    Math::Vector3d result = a + b;
    CHECK_APPROX(result.x, 5.0);
    CHECK_APPROX(result.y, 7.0);
    CHECK_APPROX(result.z, 9.0);
}

TEST_CASE("Point3d subtraction is not commutative", "[point3d]") {
    Math::Point3d a{5,0,0};
    Math::Point3d b{2,0,0};
    Math::Vector3d ab = a - b;
    Math::Vector3d ba = b - a;
    CHECK_APPROX(ab.x,  3.0);
    CHECK_APPROX(ba.x, -3.0);
}
