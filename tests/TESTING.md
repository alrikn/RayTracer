# Raytracer Test Guide

This file is the single source of truth for how testing works in this project.
Read this before writing any new test. It takes ~5 minutes.

---

## Quick start

```bash
make test              # build + run all tests — prints final summary per tier
make test_unit         # fast isolated tests only (no I/O, no Scene)
make test_integration  # Scene::traceRay() end-to-end tests
make test_functional   # full renders, pixel-level checks
make coverage          # build with instrumentation, run all tests, generate HTML report + lcov.info
```

A passing `make test` ends with:
```
══════════════════════════════════════════════════════════════════════════════
  TEST RESULTS
  ALL TESTS PASSED
══════════════════════════════════════════════════════════════════════════════
  Unit        │ test cases:  74 |  74 passed | 0 failed | 0 skipped
  Integration │ test cases:  11 |  11 passed | 0 failed | 0 skipped
  Functional  │ test cases:   7 |   7 passed | 0 failed | 0 skipped
══════════════════════════════════════════════════════════════════════════════
```

`make coverage` compiles all objects with `-fprofile-instr-generate -fcoverage-mapping`, runs the three test binaries to collect profiling data, merges it, and produces:
- `coverage_report/index.html` — browsable line-by-line HTML report
- `lcov.info` — machine-readable summary (used by the CI workflow)

It prints the same per-tier TEST RESULTS summary at the end, then deletes the instrumented `.o` files so a subsequent `make` always rebuilds clean non-instrumented objects.

If any tier fails, `make test` still runs the remaining tiers, shows the doctest failure output for the failing tier(s), and prints the final summary banner at the end.

---

## How the test system is organized

```
tests/
├── doctest.h              Single-header framework (v2.4.11) — never edit
├── helpers.hpp            Shared macros + builders — include in every test file
├── TESTING.md             This file
│
├── unit/                  One class per file. No Scene. No disk I/O. Very fast.
│   ├── runner.cpp         Defines main() — never add TEST_CASEs here
│   ├── test_vector3d.cpp
│   ├── test_point3d.cpp
│   ├── test_ray.cpp
│   ├── test_camera.cpp
│   ├── test_sphere.cpp
│   ├── test_plane.cpp
│   ├── test_cylinder.cpp
│   ├── test_cone.cpp
│   ├── test_ambient_light.cpp
│   ├── test_directional_light.cpp
│   └── test_specular_light.cpp
│
├── integration/           Test Scene::traceRay() with objects built in code. No parser.
│   ├── runner.cpp
│   ├── test_scene_ambient.cpp      ← empty scene, sphere+ambient, miss, brightness
│   ├── test_scene_shadows.cpp      ← directional light, shadow with plane+blocker
│   └── test_scene_reflections.cpp  ← max_depth guard, reflectivity, recursive mirrors
│
└── functional/            Call scene.render(), parse PPM output, check pixel statistics.
    ├── runner.cpp
    ├── test_render_shapes.cpp      ← empty scene, sphere, plane, cylinder, cone
    ├── test_render_lighting.cpp    ← directional light gradient
    ├── test_render_aa.cpp          ← Supersampling(4), Supersampling(9), AA≠no-AA
    └── test_render_reflections.cpp ← reflective sphere shows reflected color
```

**Rule:** Each tier has exactly one `runner.cpp` that defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`. All other `.cpp` files just `#include "helpers.hpp"` and write `TEST_CASE` blocks.

**File-per-feature rule:** One file per class (unit) or per feature/topic (integration/functional). The Makefile uses `$(wildcard ...)`, so any new `.cpp` file is picked up automatically — no Makefile edit needed.

---

## How to add a test for a new shape

1. Create `tests/unit/test_<shapename>.cpp`.
2. Copy `test_sphere.cpp` as the template.
3. Write at minimum three `TEST_CASE` blocks:

```cpp
#include "helpers.hpp"
#include "MyNewShape.hpp"

TEST_CASE("MyNewShape: ray hits it", "[mynewshape]") {
    RayTracer::MyNewShape s{/* constructor args */};
    auto hit = s.hits(make_ray(ox, oy, oz,  dx, dy, dz));
    REQUIRE(hit.has_value());
    CHECK_APPROX(hit->distance, expected_distance);  // compute by hand from the math
    CHECK_VEC3(hit->normal, nx, ny, nz);
}

TEST_CASE("MyNewShape: ray misses", "[mynewshape]") {
    RayTracer::MyNewShape s{/* ... */};
    CHECK(!s.hits(make_ray(/* far-away ray */)).has_value());
}

TEST_CASE("MyNewShape: edge case (tangent / inside / back-face)", "[mynewshape]") {
    // ...
}
```

4. Run `make test_unit` — picked up automatically.

---

## How to add a test for a new light

1. Create `tests/unit/test_<lightname>_light.cpp`.
2. Use `make_hit()` from `helpers.hpp` to build a `HitRecord` without tracing a real ray.
3. **Always pass a non-empty objects vector.** `AmbientLight::intensity()` returns black
   when `objects.empty()` (implementation quirk). Use a far-away dummy sphere.

```cpp
#include "helpers.hpp"
#include "MyNewLight.hpp"
#include "Sphere.hpp"

// Non-empty objects list required — see AmbientLight quirk in helpers.hpp
static auto one_object() {
    return std::vector<std::shared_ptr<RayTracer::IShape>>{
        std::make_shared<RayTracer::Sphere>(Math::Point3d{0,1000,0}, 0.1)
    };
}

TEST_CASE("MyNewLight: basic illumination", "[mynewlight]") {
    RayTracer::MyNewLight light{/* args */};
    auto hit = make_hit(0,0,0,  0,1,0,  255,0,0,  0,-1,0);
    auto result = light.intensity(hit, one_object());
    CHECK_COLOR_VEC3(result, expected_r, expected_g, expected_b);
}

TEST_CASE("MyNewLight: shadow blocks light", "[mynewlight]") {
    RayTracer::MyNewLight light{/* ... */};
    auto hit = make_hit(0,0,0, 0,1,0, 255,0,0);
    auto blocker = std::make_shared<RayTracer::Sphere>(Math::Point3d{0,2,0}, 0.5);
    auto result = light.intensity(hit, {blocker});
    CHECK_COLOR_VEC3(result, 0.0, 0.0, 0.0);
}
```

---

## How to add a functional render test

Pick the right file for your feature and add a `TEST_CASE` block. Each file has a
`render_10x10(scene)` or `render_5x5(scene)` helper that returns a flat pixel vector.

```cpp
TEST_CASE("Golden render: my new feature", "[functional]") {
    RayTracer::Scene scene{1.0, 4};
    // ... add shapes and lights ...
    auto pixels = render_10x10(scene);

    int max_red = 0;
    for (auto& p : pixels) max_red = std::max(max_red, p[0]);
    CHECK(max_red > 200);
}
```

**Why 5×5 for reflection tests:** A 10×10 grid never places a pixel exactly at screen
center (u=0.5, v=0.5). For tests where the reflected ray must hit a specific small target,
use 5×5 — pixel (2,2) gives u=0.5, v=0.5 exactly → direction (0,0,−1).

**Why no exact pixel values:** Multi-threading and float rounding make exact values
fragile. Use statistical checks: max channel, corner pixels, row sums, render A vs render B.

---

## How to add a test for a new AA technique

1. Add `#include "AntiAliasing/MyNewAA.hpp"` in `tests/functional/test_render_aa.cpp`.
2. Copy one of the Supersampling tests.
3. Replace `make_unique<RayTracer::Supersampling>(4)` with `make_unique<RayTracer::MyNewAA>(args)`.
4. Keep the same assertions — they test the `IAntiAliasing` interface, not the algorithm.

---

## Floating-point rules — IMPORTANT

**Never use `==` to compare doubles.** Always use the macros from `helpers.hpp`:

| What you're comparing | Macro | Tolerance |
|---|---|---|
| distances, dot products, normals | `CHECK_APPROX(a, b)` | relative 1e-5 |
| RGB color values (0–255 range) | `CHECK_COLOR(a, b)` | absolute ±1.5 |
| Vector3d geometry | `CHECK_VEC3(v, x, y, z)` | relative 1e-5 each |
| Vector3d color | `CHECK_COLOR_VEC3(v, x, y, z)` | absolute ±1.5 each |

`CHECK_APPROX` uses `doctest::Approx().epsilon()` (relative tolerance).
`CHECK_COLOR` uses `std::abs(a - b) <= 1.5` (absolute tolerance) — doctest v2.4.11
has no `.margin()` so we use direct comparison.

---

## Known implementation quirks that affect tests

- **AmbientLight empty-vector bug:** `intensity()` returns black when `objects` is empty.
  Always pass at least one shape. See `dummy_objects()` in `test_ambient_light.cpp`.

- **DirectionalLight color shadowing bug:** `DirectionalLight` has its own `_color` member
  that shadows `ALight::_color`. The color passed to the constructor is silently ignored —
  it always uses white. Tests use white light to work around this.

- **Ray::Ray() default constructor:** Declared but never defined. Always use the two-arg
  constructor: `Ray{Point3d{...}, Vector3d{...}}`.

---

## What is NOT tested here

- **Parser** (`src/Parser/`): depends on libconfig++ and reads disk files. Tested
  implicitly by running the binary with a `.cfg` file.
- **Rectangle**: `hits()` is not implemented (returns `nullopt` always).
- **Multiple lights combined**: `average_light()` with ambient + directional + specular.
- **Reflections of reflections** (beyond termination): only tested that recursion stops.
