/*
** Additional tests for Math::Vector3d — covers previously uncovered lines:
**   - normalize() zero-vector fallback (line 96)
**   - operator*=(Vector3d) body (lines 146-149)
**   - operator/=(Vector3d) body (lines 152-155)
*/
#include "helpers.hpp"
#include "Math/Vector3d.hpp"

TEST_CASE("Vector3d::normalize zero vector returns zero vector", "[vector3d]") {
    Math::Vector3d v{0, 0, 0};
    auto n = v.normalize();
    CHECK_VEC3(n, 0.0, 0.0, 0.0);
}

TEST_CASE("Vector3d compound assignment operators with vector operand", "[vector3d]") {
    SUBCASE("*= Vector3d component-wise") {
        Math::Vector3d a{2, 3, 4};
        a *= Math::Vector3d{1, 2, 3};
        CHECK_VEC3(a, 2.0, 6.0, 12.0);
    }
    SUBCASE("/= Vector3d component-wise") {
        Math::Vector3d a{4, 6, 8};
        a /= Math::Vector3d{2, 3, 4};
        CHECK_VEC3(a, 2.0, 2.0, 2.0);
    }
}
