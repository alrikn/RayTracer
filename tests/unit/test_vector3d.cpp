/*
** Tests for Math::Vector3d
**
** HOW TO ADD A NEW TEST:
**   TEST_CASE("short description", "[vector3d]") {
**       Math::Vector3d v{1, 2, 3};       // always assign to a variable first
**       CHECK_APPROX(v.length(), expected);  // then pass variable to macro
**   }
** Important: never put Type{...} braced-init directly inside CHECK_APPROX() — the
** commas are parsed as macro argument separators. Always use a local variable.
*/
#include "helpers.hpp"
#include "Math/Vector3d.hpp"

// ── Construction & fields ─────────────────────────────────────────────────────

TEST_CASE("Vector3d default constructor initializes to zero", "[vector3d]") {
    Math::Vector3d v;
    CHECK_APPROX(v.x, 0.0);
    CHECK_APPROX(v.y, 0.0);
    CHECK_APPROX(v.z, 0.0);
}

TEST_CASE("Vector3d explicit constructor stores values", "[vector3d]") {
    Math::Vector3d v{1.0, 2.5, -3.0};
    CHECK_APPROX(v.x, 1.0);
    CHECK_APPROX(v.y, 2.5);
    CHECK_APPROX(v.z, -3.0);
}

// ── length() ─────────────────────────────────────────────────────────────────

TEST_CASE("Vector3d::length", "[vector3d]") {
    SUBCASE("zero vector") {
        Math::Vector3d v{0,0,0};
        CHECK_APPROX(v.length(), 0.0);
    }
    SUBCASE("axis vector 3-4-0 gives 5 (3-4-5 right triangle)") {
        Math::Vector3d v{3,4,0};
        CHECK_APPROX(v.length(), 5.0);
    }
    SUBCASE("3D vector 1-2-2 gives 3") {
        Math::Vector3d v{1,2,2};
        CHECK_APPROX(v.length(), 3.0);
    }
    SUBCASE("negative components have same length") {
        Math::Vector3d v{-1,-2,-2};
        CHECK_APPROX(v.length(), 3.0);
    }
}

// ── dot() ─────────────────────────────────────────────────────────────────────

TEST_CASE("Vector3d::dot", "[vector3d]") {
    SUBCASE("perpendicular vectors have zero dot product") {
        Math::Vector3d a{1,0,0}, b{0,1,0};
        CHECK_APPROX(a.dot(b), 0.0);
    }
    SUBCASE("parallel vectors: dot equals product of lengths") {
        Math::Vector3d a{2,0,0}, b{3,0,0};
        CHECK_APPROX(a.dot(b), 6.0);
    }
    SUBCASE("dot with self equals squared length") {
        Math::Vector3d v{1,2,3};
        CHECK_APPROX(v.dot(v), 14.0);
    }
    SUBCASE("commutative: a.dot(b) == b.dot(a)") {
        Math::Vector3d a{1,2,3}, b{4,5,6};
        CHECK_APPROX(a.dot(b), b.dot(a));
    }
}

// ── cross() ──────────────────────────────────────────────────────────────────

TEST_CASE("Vector3d::cross", "[vector3d]") {
    SUBCASE("X cross Y gives +Z") {
        Math::Vector3d x{1,0,0}, y{0,1,0};
        auto c = x.cross(y);
        CHECK_VEC3(c, 0.0, 0.0, 1.0);
    }
    SUBCASE("Y cross X gives -Z (anti-commutative)") {
        Math::Vector3d y{0,1,0}, x{1,0,0};
        auto c = y.cross(x);
        CHECK_VEC3(c, 0.0, 0.0, -1.0);
    }
    SUBCASE("Y cross Z gives +X") {
        Math::Vector3d y{0,1,0}, z{0,0,1};
        auto c = y.cross(z);
        CHECK_VEC3(c, 1.0, 0.0, 0.0);
    }
    SUBCASE("cross product is perpendicular to both inputs") {
        Math::Vector3d a{1,2,3}, b{4,5,6};
        auto c = a.cross(b);
        CHECK_APPROX(c.dot(a), 0.0);
        CHECK_APPROX(c.dot(b), 0.0);
    }
    SUBCASE("cross with self gives zero vector") {
        Math::Vector3d v{1,2,3};
        auto c = v.cross(v);
        CHECK_APPROX(c.length(), 0.0);
    }
}

// ── normalize() ──────────────────────────────────────────────────────────────

TEST_CASE("Vector3d::normalize", "[vector3d]") {
    SUBCASE("axis vector normalizes to unit vector") {
        Math::Vector3d v{5,0,0};
        auto n = v.normalize();
        CHECK_VEC3(n, 1.0, 0.0, 0.0);
    }
    SUBCASE("3-4-5 vector normalizes to 0.6-0.8-0") {
        Math::Vector3d v{3,4,0};
        auto n = v.normalize();
        CHECK_APPROX(n.x, 0.6);
        CHECK_APPROX(n.y, 0.8);
        CHECK_APPROX(n.z, 0.0);
    }
    SUBCASE("normalized vector has length 1") {
        Math::Vector3d v{1,2,3};
        auto n = v.normalize();
        CHECK_APPROX(n.length(), 1.0);
    }
    SUBCASE("normalize does not modify original (returns copy)") {
        Math::Vector3d v{5,0,0};
        auto n = v.normalize();
        CHECK_APPROX(v.x, 5.0);  // original unchanged
        CHECK_APPROX(n.x, 1.0);  // copy is normalized
    }
}

TEST_CASE("Vector3d::normalizeSelf modifies in place", "[vector3d]") {
    Math::Vector3d v{3,4,0};
    v.normalizeSelf();
    CHECK_APPROX(v.length(), 1.0);
    CHECK_APPROX(v.x, 0.6);
    CHECK_APPROX(v.y, 0.8);
}

// ── translate() ───────────────────────────────────────────────────────────────

TEST_CASE("Vector3d::translate adds vector in place", "[vector3d]") {
    Math::Vector3d v{1,2,3};
    v.translate(Math::Vector3d{10,20,30});
    CHECK_VEC3(v, 11.0, 22.0, 33.0);
}

// ── Rotations ─────────────────────────────────────────────────────────────────

TEST_CASE("Vector3d rotation by 90 degrees", "[vector3d]") {
    const double PI_2 = M_PI / 2.0;

    SUBCASE("rotateZ: X-axis becomes Y-axis") {
        Math::Vector3d v{1,0,0};
        v.rotateZ(PI_2);
        CHECK_APPROX(v.x, 0.0);
        CHECK_APPROX(v.y, 1.0);
        CHECK_APPROX(v.z, 0.0);
    }
    SUBCASE("rotateX: Y-axis becomes Z-axis") {
        Math::Vector3d v{0,1,0};
        v.rotateX(PI_2);
        CHECK_APPROX(v.x, 0.0);
        CHECK_APPROX(v.y, 0.0);
        CHECK_APPROX(v.z, 1.0);
    }
    SUBCASE("rotateY: Z-axis becomes X-axis") {
        Math::Vector3d v{0,0,1};
        v.rotateY(PI_2);
        CHECK_APPROX(v.x, 1.0);
        CHECK_APPROX(v.y, 0.0);
        CHECK_APPROX(v.z, 0.0);
    }
}

// ── Arithmetic operators ──────────────────────────────────────────────────────

TEST_CASE("Vector3d operator+", "[vector3d]") {
    Math::Vector3d a{1,2,3}, b{4,5,6};
    auto r = a + b;
    CHECK_VEC3(r, 5.0, 7.0, 9.0);
}

TEST_CASE("Vector3d operator-", "[vector3d]") {
    Math::Vector3d a{5,7,9}, b{4,5,6};
    auto r = a - b;
    CHECK_VEC3(r, 1.0, 2.0, 3.0);
}

TEST_CASE("Vector3d operator* (vector * vector, component-wise)", "[vector3d]") {
    Math::Vector3d a{2,3,4}, b{1,2,3};
    auto r = a * b;
    CHECK_VEC3(r, 2.0, 6.0, 12.0);
}

TEST_CASE("Vector3d operator/ (vector / vector, component-wise)", "[vector3d]") {
    Math::Vector3d a{4,6,8}, b{2,3,4};
    auto r = a / b;
    CHECK_VEC3(r, 2.0, 2.0, 2.0);
}

TEST_CASE("Vector3d operator* (vector * scalar)", "[vector3d]") {
    Math::Vector3d v{1,2,3};
    auto r = v * 2.0;
    CHECK_VEC3(r, 2.0, 4.0, 6.0);
}

TEST_CASE("Vector3d operator/ (vector / scalar)", "[vector3d]") {
    Math::Vector3d v{4,6,8};
    auto r = v / 2.0;
    CHECK_VEC3(r, 2.0, 3.0, 4.0);
}

TEST_CASE("Vector3d compound assignment operators", "[vector3d]") {
    SUBCASE("+=") {
        Math::Vector3d v{1,2,3};
        v += Math::Vector3d{4,5,6};
        CHECK_VEC3(v, 5.0, 7.0, 9.0);
    }
    SUBCASE("-=") {
        Math::Vector3d v{5,7,9};
        v -= Math::Vector3d{4,5,6};
        CHECK_VEC3(v, 1.0, 2.0, 3.0);
    }
    SUBCASE("*= scalar") {
        Math::Vector3d v{1,2,3};
        v *= 3.0;
        CHECK_VEC3(v, 3.0, 6.0, 9.0);
    }
    SUBCASE("/= scalar") {
        Math::Vector3d v{2,4,6};
        v /= 2.0;
        CHECK_VEC3(v, 1.0, 2.0, 3.0);
    }
}
