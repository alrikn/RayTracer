// /*
// ** Tests for RayTracer::SpecularLight::intensity()
// **
// ** Phong specular model:
// **   light_dir  = -_direction              (toward light source)
// **   view_dir   = -hit.incomingDirection   (toward viewer)
// **   reflect    = normalize(2*(normal·light_dir)*normal - light_dir)
// **   spec_angle = max(0, reflect · view_dir)
// **   spec       = spec_angle^shininess * specular_strength
// **   result     = light_color * intensity * spec
// **
// ** Note: SpecularLight does NOT modulate with hit.color — it is a pure highlight.
// **
// ** HOW TO SET UP A PERFECT-ALIGNMENT SPECULAR TEST:
// **   Light from above (dir (0,-1,0)), surface normal up (0,1,0),
// **   ray coming from above (incomingDir (0,-1,0)).
// **   → reflect = (0,1,0), view = (0,1,0), spec_angle = 1.0.
// */
// #include "helpers.hpp"
// #include "SpecularLight.hpp"
// #include "Sphere.hpp"
// #include <cmath>

// // ── Specular highlight ────────────────────────────────────────────────────────

// TEST_CASE("SpecularLight: perfect mirror alignment gives maximum specular", "[specular_light]") {
//     // light dir (0,-1,0): light comes from above.
//     // normal (0,1,0):     surface faces up.
//     // incomingDir (0,-1,0): ray came from above (same as light direction).
//     //
//     // view_dir   = -(0,-1,0)         = (0,1,0)
//     // light_dir  = -(0,-1,0)         = (0,1,0)
//     // normal·light_dir = 1.0
//     // reflect    = normalize(2*1*(0,1,0) - (0,1,0)) = normalize(0,1,0) = (0,1,0)
//     // spec_angle = (0,1,0)·(0,1,0)  = 1.0
//     // spec       = 1.0^32 * 0.5     = 0.5
//     // result     = white * 1.0 * 0.5 = (127.5, 127.5, 127.5)
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0,-1,0}, 1.0, 32.0, 0.5, Math::Vector3d{255,255,255}
//     };
//     auto hit = make_hit(0,0,0, 0,1,0, 255,0,0, 0,-1,0);
//     auto result = light.intensity(hit, {});
//     CHECK_COLOR_VEC3(result, 127.5, 127.5, 127.5);
// }

// TEST_CASE("SpecularLight: off-axis view (90 degrees from reflect) gives zero", "[specular_light]") {
//     // light dir (0,-1,0), normal (0,1,0), but ray came from the SIDE (1,0,0).
//     // view_dir = (-1,0,0).
//     // reflect = (0,1,0) (same as perfect case).
//     // spec_angle = (0,1,0)·(-1,0,0) = 0.0 → result = (0,0,0)
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0,-1,0}, 1.0, 32.0, 0.5, Math::Vector3d{255,255,255}
//     };
//     auto hit = make_hit(0,0,0, 0,1,0, 255,0,0, 1,0,0);
//     auto result = light.intensity(hit, {});
//     CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
// }

// TEST_CASE("SpecularLight: back-face surface gives zero (no light)", "[specular_light]") {
//     // normal (0,-1,0) faces away from light coming from above.
//     // normal·light_dir = (0,-1,0)·(0,1,0) = -1 ≤ 0 → early return (0,0,0).
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0,-1,0}, 1.0, 32.0, 0.5, Math::Vector3d{255,255,255}
//     };
//     auto hit = make_hit(0,0,0, 0,-1,0, 255,0,0, 0,-1,0);
//     auto result = light.intensity(hit, {});
//     CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
// }

// // ── Shadow detection ──────────────────────────────────────────────────────────

// TEST_CASE("SpecularLight: blocking sphere casts shadow (result is black)", "[specular_light]") {
//     // Same perfect-alignment setup, but a sphere sits between hit.point and the light.
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0,-1,0}, 1.0, 32.0, 0.5, Math::Vector3d{255,255,255}
//     };
//     auto hit = make_hit(0,0,0, 0,1,0, 255,0,0, 0,-1,0);
//     auto blocker = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,2,0}, 0.5);
//     auto result = light.intensity(hit, {blocker});
//     CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
// }

// // ── Shininess parameter ───────────────────────────────────────────────────────

// TEST_CASE("SpecularLight: higher shininess gives a smaller highlight", "[specular_light]") {
//     // Use a slightly off-axis view to see shininess effect.
//     // Ray from slightly to the side — not perfect alignment, so shininess matters.
//     // spec_angle = cos(10°) ≈ 0.985.
//     // shininess=8:  0.985^8  ≈ 0.881
//     // shininess=64: 0.985^64 ≈ 0.388
//     // Higher shininess → smaller spec value.
//     RayTracer::SpecularLight low_shine{
//         Math::Vector3d{0,-1,0}, 1.0, 8.0, 1.0, Math::Vector3d{255,0,0}
//     };
//     RayTracer::SpecularLight high_shine{
//         Math::Vector3d{0,-1,0}, 1.0, 64.0, 1.0, Math::Vector3d{255,0,0}
//     };
//     // incomingDir slightly off perfect — normal (0,1,0), light from above,
//     // ray came from slight angle (sin10, -cos10, 0)
//     auto hit = make_hit(0,0,0, 0,1,0, 255,0,0,
//                         std::sin(0.1745), -std::cos(0.1745), 0.0);

//     auto r_low  = low_shine.intensity(hit, {});
//     auto r_high = high_shine.intensity(hit, {});

//     // Higher shininess should produce a smaller (or equal) red component
//     CHECK(r_high.x <= r_low.x + COLOR_EPS);
// }

// TEST_CASE("SpecularLight: zero shininess returns black immediately", "[specular_light]") {
//     // Covers line 30: if(_shininess <= 0.0) guard → return (0,0,0).
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0, -1, 0}, 1.0, 0.0, 0.5, Math::Vector3d{255, 255, 255}
//     };
//     auto hit = make_hit(0, 0, 0,  0, 1, 0,  255, 0, 0,  0, -1, 0);
//     auto result = light.intensity(hit, {});
//     CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
// }

// TEST_CASE("SpecularLight: zero specular_strength returns black immediately", "[specular_light]") {
//     // Covers line 30: if(_specular_strength <= 0.0) guard → return (0,0,0).
//     RayTracer::SpecularLight light{
//         Math::Vector3d{0, -1, 0}, 1.0, 32.0, 0.0, Math::Vector3d{255, 255, 255}
//     };
//     auto hit = make_hit(0, 0, 0,  0, 1, 0,  255, 0, 0,  0, -1, 0);
//     auto result = light.intensity(hit, {});
//     CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
// }
