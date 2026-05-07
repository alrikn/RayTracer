// /*
// ** Tests for RayTracer::Sphere::hits()
// **
// ** Sphere at center C with radius r. A ray from origin O in direction D hits
// ** the sphere by solving: |O + t*D - C|² = r² (quadratic in t).
// **
// ** HOW TO ADD A NEW SPHERE TEST:
// **   1. Build a sphere: Sphere s{Math::Point3d{cx,cy,cz}, radius};
// **   2. Build a ray:    auto ray = make_ray(ox,oy,oz, dx,dy,dz);
// **   3. Call:           auto hit = s.hits(ray);
// **   4. Check:          REQUIRE(hit.has_value()); or CHECK(!hit.has_value());
// **      Then:           CHECK_APPROX(hit->distance, expected_dist);
// **                      CHECK_VEC3(hit->normal, nx, ny, nz);
// */
// #include "helpers.hpp"
// #include "Sphere.hpp"

// // ── Direct hits ───────────────────────────────────────────────────────────────

// TEST_CASE("Sphere: ray hits front surface", "[sphere]") {
//     // Sphere at (0,0,0) r=1. Ray from (0,0,5) pointing toward -Z.
//     // Hits front surface (z=+1) at t=4.
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     auto hit = s.hits(make_ray(0,0,5, 0,0,-1));

//     REQUIRE(hit.has_value());
//     CHECK_APPROX(hit->distance, 4.0);
//     CHECK_APPROX(hit->point.x, 0.0);
//     CHECK_APPROX(hit->point.y, 0.0);
//     CHECK_APPROX(hit->point.z, 1.0);
//     // Normal at front surface points away from center (+Z)
//     CHECK_APPROX(hit->normal.z, 1.0);
// }

// TEST_CASE("Sphere: ray hits back surface", "[sphere]") {
//     // Same sphere, ray from the other side.
//     // From (0,0,-5) toward +Z: hits back surface (z=-1) at t=4.
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     auto hit = s.hits(make_ray(0,0,-5, 0,0,1));

//     REQUIRE(hit.has_value());
//     CHECK_APPROX(hit->distance, 4.0);
//     CHECK_APPROX(hit->point.z, -1.0);
//     CHECK_APPROX(hit->normal.z, -1.0);
// }

// TEST_CASE("Sphere: HitRecord stores incoming ray direction", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     auto ray = make_ray(0,0,5, 0,0,-1);
//     auto hit = s.hits(ray);

//     REQUIRE(hit.has_value());
//     CHECK_APPROX(hit->incomingDirection.x, ray.direction.x);
//     CHECK_APPROX(hit->incomingDirection.y, ray.direction.y);
//     CHECK_APPROX(hit->incomingDirection.z, ray.direction.z);
// }

// TEST_CASE("Sphere: HitRecord color matches sphere color", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     s.setColor(GREEN);
//     auto hit = s.hits(make_ray(0,0,5, 0,0,-1));

//     REQUIRE(hit.has_value());
//     CHECK_COLOR(hit->color.x, 0.0);
//     CHECK_COLOR(hit->color.y, 255.0);
//     CHECK_COLOR(hit->color.z, 0.0);
// }

// // ── Misses ────────────────────────────────────────────────────────────────────

// TEST_CASE("Sphere: ray misses completely", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     // Ray passes 5 units to the side — well outside radius 1
//     auto hit = s.hits(make_ray(5,0,0, 0,0,-1));
//     CHECK(!hit.has_value());
// }

// TEST_CASE("Sphere: ray pointing away from sphere misses", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,-5}, 1.0};
//     // Sphere is at -Z, ray points +Z (away from it)
//     auto hit = s.hits(make_ray(0,0,0, 0,0,1));
//     CHECK(!hit.has_value());
// }

// // ── Edge cases ────────────────────────────────────────────────────────────────

// TEST_CASE("Sphere: tangent ray (discriminant=0) registers a hit", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     // Ray passes exactly at y=1 (tangent to equator)
//     auto hit = s.hits(make_ray(0,1,5, 0,0,-1));
//     // discriminant = 0 → one intersection point, should still register
//     CHECK(hit.has_value());
// }

// TEST_CASE("Sphere: ray starting inside sphere hits exit surface", "[sphere]") {
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 5.0};
//     // Origin is inside the sphere — ray exits through front
//     auto hit = s.hits(make_ray(0,0,0, 0,0,1));
//     REQUIRE(hit.has_value());
//     CHECK(hit->distance > 0.0);
// }

// TEST_CASE("Sphere: normal is outward-facing at hit point", "[sphere]") {
//     // For a front hit from +Z, the normal at the surface must point toward +Z
//     RayTracer::Sphere s{Math::Point3d{0,0,0}, 1.0};
//     auto hit = s.hits(make_ray(0,0,5, 0,0,-1));
//     REQUIRE(hit.has_value());
//     // The hit is at the front surface — normal points away from sphere center
//     // normal · (hit.point - sphere.center) should be positive
//     Math::Vector3d to_surface = hit->point - Math::Point3d{0,0,0};
//     CHECK(hit->normal.dot(to_surface) > 0.0);
// }
