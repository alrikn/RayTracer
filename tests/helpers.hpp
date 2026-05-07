/*
** Shared test helpers for all raytracer tests.
**
** USAGE: #include "helpers.hpp" at the top of every test file.
**        Do NOT define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN here — only runner.cpp does that.
**
** ADDING A NEW HELPER: add it here if it's useful across 3+ test files.
** Keep shape-specific setup code inside the test file that uses it.
*/
#pragma once
#include "doctest.h"

// doctest v2.4.11's TEST_CASE macro only accepts one argument (the name).
// This shim makes TEST_CASE("name", "[tag]") work by embedding the tag in
// the name: TEST_CASE("[tag] name"). Filtering still works via -tc="*tag*".
#ifdef TEST_CASE
#undef TEST_CASE
#endif
#define DOCTEST_TC_1(name)       DOCTEST_TEST_CASE(name)
#define DOCTEST_TC_2(name, tags) DOCTEST_TEST_CASE(tags " " name)
#define DOCTEST_TC_SEL(_1, _2, X, ...) X
#define TEST_CASE(...) DOCTEST_TC_SEL(__VA_ARGS__, DOCTEST_TC_2, DOCTEST_TC_1)(__VA_ARGS__)

#include <cmath>
#include "Math/Vector3d.hpp"
#include "Math/Point3d.hpp"
#include "Ray.hpp"
#include "Shape/IShape.hpp"  // pulls in HitRecord, IShape, Enums

// ── Tolerances ────────────────────────────────────────────────────────────────
// GEOM_EPS: relative tolerance for geometry — doctest Approx.epsilon() is
//           relative: tolerance = eps * max(|a|, |b|, 1). Good for distances.
// COLOR_EPS: ABSOLUTE tolerance for 0-255 color range. doctest v2.4.11 has
//           no .margin() so we use std::abs directly. ±1.5 per channel.
constexpr double GEOM_EPS  = 1e-5;
constexpr double COLOR_EPS = 1.5;

// ── Assertion macros ──────────────────────────────────────────────────────────
// Always use these instead of bare == on doubles.

#define CHECK_APPROX(a, b)   CHECK((a) == doctest::Approx(b).epsilon(GEOM_EPS))
#define REQUIRE_APPROX(a, b) REQUIRE((a) == doctest::Approx(b).epsilon(GEOM_EPS))

// Absolute-tolerance check for colors in 0-255 range.
// std::abs is used because doctest v2.4.11 has no Approx.margin().
#define CHECK_COLOR(a, b) \
    CHECK(std::abs(static_cast<double>(a) - static_cast<double>(b)) <= COLOR_EPS)

// Component-wise Vector3d checks (geometry precision)
#define CHECK_VEC3(v, ex, ey, ez) \
    CHECK_APPROX((v).x, (ex));    \
    CHECK_APPROX((v).y, (ey));    \
    CHECK_APPROX((v).z, (ez))

// Component-wise Vector3d checks (color precision — ±COLOR_EPS per channel)
#define CHECK_COLOR_VEC3(v, ex, ey, ez) \
    CHECK_COLOR((v).x, (ex));           \
    CHECK_COLOR((v).y, (ey));           \
    CHECK_COLOR((v).z, (ez))

// ── Object builders ───────────────────────────────────────────────────────────
// Use these to build minimal objects for testing without going through Parser.

inline RayTracer::Ray make_ray(double ox, double oy, double oz,
                               double dx, double dy, double dz)
{
    return RayTracer::Ray{Math::Point3d{ox, oy, oz}, Math::Vector3d{dx, dy, dz}};
}

// Build a HitRecord manually — used for lighting tests where you don't need
// to actually trace a ray. All field values are explicit.
//
// point   — 3D intersection position (used by DirectionalLight shadow ray)
// normal  — outward surface normal
// color   — object color in 0-255 range
// indir   — direction of the ray that caused the hit (used by SpecularLight)
inline RayTracer::HitRecord make_hit(
    double px, double py, double pz,
    double nx, double ny, double nz,
    double cr, double cg, double cb,
    double idx = 0.0, double idy = -1.0, double idz = 0.0)
{
    return RayTracer::HitRecord{
        .distance          = 1.0,
        .point             = Math::Point3d{px, py, pz},
        .normal            = Math::Vector3d{nx, ny, nz},
        .color             = Math::Vector3d{cr, cg, cb},
        .incomingDirection = Math::Vector3d{idx, idy, idz}
    };
}
