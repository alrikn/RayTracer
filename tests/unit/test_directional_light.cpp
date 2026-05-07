/*
** Tests for RayTracer::DirectionalLight::intensity()
**
** Formula: result = light_color * intensity * lambert * (hit.color / 255.0)
**   where lambert = max(0, hit.normal · light_direction)
**   and   light_direction = -direction  (pointing TOWARD the light source)
**
** Shadow: if any object lies between hit.point and the light, returns (0,0,0).
**
** HOW TO ADD A SHADOW TEST:
**   1. Place hit.point away from the blocking object
**   2. Add the blocking object to the `objects` vector
**   3. Confirm the result is (0,0,0) — fully in shadow
*/
#include "helpers.hpp"
#include "DirectionalLight.hpp"
#include "Sphere.hpp"

// ── Lambert reflectance ───────────────────────────────────────────────────────

TEST_CASE("DirectionalLight: perpendicular incidence gives full brightness", "[directional_light]") {
    // Light coming from above (direction (0,-1,0)), hitting a surface with normal (0,1,0).
    // lambert = (0,1,0) · (0,1,0) = 1.0  → full intensity.
    // Result = white * 1.0 * 1.0 * red/255 = red (255, 0, 0).
    RayTracer::DirectionalLight light{Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto result = light.intensity(hit, {});
    CHECK_COLOR_VEC3(result, 255.0, 0.0, 0.0);
}

TEST_CASE("DirectionalLight: 45-degree incidence reduces brightness", "[directional_light]") {
    // Light at 45° from horizontal. lambert ≈ 0.707.
    // Result ≈ 255 * 0.707 ≈ 180 (just checking R channel for red object).
    Math::Vector3d diag{-1,-1,0};
    diag.normalizeSelf();
    RayTracer::DirectionalLight light{diag, 1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto result = light.intensity(hit, {});

    const double lambert_45 = 1.0 / std::sqrt(2.0);
    CHECK_COLOR(result.x, 255.0 * lambert_45);
    CHECK_COLOR(result.y, 0.0);
    CHECK_COLOR(result.z, 0.0);
}

TEST_CASE("DirectionalLight: back-face surface (normal facing away) gives black", "[directional_light]") {
    // Normal (0,-1,0) faces away from the light coming from above.
    // lambert = (0,-1,0) · (0,1,0) = -1 → clamped to 0 → result = black.
    RayTracer::DirectionalLight light{Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,-1,0, 255,0,0);
    auto result = light.intensity(hit, {});
    CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
}

// ── Shadow detection ──────────────────────────────────────────────────────────

TEST_CASE("DirectionalLight: no shadow when objects list is empty", "[directional_light]") {
    RayTracer::DirectionalLight light{Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto result = light.intensity(hit, {});
    // Should not be black (no blocker)
    CHECK(result.x > 0.0);
}

TEST_CASE("DirectionalLight: blocking sphere casts shadow (result is black)", "[directional_light]") {
    // Hit point at (0,0,0), normal (0,1,0). Light from above (direction (0,-1,0)).
    // Shadow ray goes from (0,0,0) toward (0,1,0).
    // Sphere at (0,2,0) radius 0.5 is directly in the path → fully shadowed.
    RayTracer::DirectionalLight light{Math::Vector3d{0,-1,0}, 1.0, Math::Vector3d{255,255,255}};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto blocker = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,2,0}, 0.5);
    auto result = light.intensity(hit, {blocker});
    CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
}
