/*
** Tests for RayTracer::Camera
**
** Camera::ray(u, v) generates a ray from the camera origin through the
** screen point at normalized coordinates (u, v) ∈ [0,1]².
**
** Default camera:
**   origin:      (0, 0, 0)
**   screen origin (bottom-left): (-2, -1, -1)
**   bottom_side: (4, 0, 0)   — horizontal span
**   left_side:   (0, 2, 0)   — vertical span
**
** So ray(u, v).direction = (-2 + 4u,  -1 + 2v,  -1)  (unnormalized).
*/
#include "helpers.hpp"
#include "Camera.hpp"

TEST_CASE("Camera::ray origin always equals camera origin", "[camera]") {
    RayTracer::Camera cam;
    auto r = cam.ray(0.5, 0.5);
    CHECK_APPROX(r.origin.x, cam.origin.x);
    CHECK_APPROX(r.origin.y, cam.origin.y);
    CHECK_APPROX(r.origin.z, cam.origin.z);
}

TEST_CASE("Camera::ray direction has negative Z for all screen positions", "[camera]") {
    RayTracer::Camera cam;
    // Z must be negative — all rays go into the scene (screen is at z = -1)
    CHECK(cam.ray(0.0, 0.0).direction.z < 0.0);
    CHECK(cam.ray(1.0, 0.0).direction.z < 0.0);
    CHECK(cam.ray(0.0, 1.0).direction.z < 0.0);
    CHECK(cam.ray(1.0, 1.0).direction.z < 0.0);
    CHECK(cam.ray(0.5, 0.5).direction.z < 0.0);
}

TEST_CASE("Camera::ray center pixel points toward screen center", "[camera]") {
    RayTracer::Camera cam;
    auto r = cam.ray(0.5, 0.5);
    // At u=0.5, v=0.5: direction = (-2+2, -1+1, -1) = (0, 0, -1)
    CHECK_APPROX(r.direction.x, 0.0);
    CHECK_APPROX(r.direction.y, 0.0);
    CHECK_APPROX(r.direction.z, -1.0);
}

TEST_CASE("Camera::ray bottom-left corner points to screen origin", "[camera]") {
    RayTracer::Camera cam;
    auto r = cam.ray(0.0, 0.0);
    // direction = screen.origin - camera.origin = (-2, -1, -1)
    CHECK_APPROX(r.direction.x, -2.0);
    CHECK_APPROX(r.direction.y, -1.0);
    CHECK_APPROX(r.direction.z, -1.0);
}

TEST_CASE("Camera::ray top-right corner points to opposite corner", "[camera]") {
    RayTracer::Camera cam;
    auto r = cam.ray(1.0, 1.0);
    // direction = (-2+4, -1+2, -1) = (2, 1, -1)
    CHECK_APPROX(r.direction.x, 2.0);
    CHECK_APPROX(r.direction.y, 1.0);
    CHECK_APPROX(r.direction.z, -1.0);
}
