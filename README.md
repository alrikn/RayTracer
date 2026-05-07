# Ray Tracer

Here is a current example of what it looks like:

![Example Render](docs/output.png)


## Description

This is an efficient raytracer written in C++.

Currently supports:
- Shapes
  - Spheres
  - Planes
  - Cylinders
  - Cones
- Lights
  - Specular
  - Directional
  - Ambient

## Build

Requires clang++ and libconfig++ (for example: `libconfig++-dev`).

```sh
make
```

## Usage

In this command, the renderer reads `scene_config.cfg` and writes a PPM image to `output.ppm`. We use ffmpeg to convert the PPM image to a PNG, since PPM is not widely supported. You can replace `scene_config.cfg` with your own scene configuration file.

Here is a command to run the raytracer and convert the output to PNG:

```sh
./raytracer scene_config.cfg -o output.ppm ; rm -f output.png; ffmpeg -i output.ppm output.png -loglevel quiet
```

## How it works

The parser loads the scene configuration, builds a `Scene` with a camera, shapes, and lights, and then renders it. For each pixel, the camera emits a ray, the renderer finds the closest intersection, and gathers light contributions from ambient, directional, and specular lights. Reflective surfaces spawn additional rays up to a maximum recursion depth to approximate reflections. The final image is written as an ASCII PPM stream.

## Performance

Here is a flame graph.


![Flame Graph](docs/flame.svg)

It can currently run 8000*4000 at 4 max depth in about 8 seconds on my laptop (my machine is a potato, so this is quite impressive). Multithreading is implemented, so it can go quite fast on a good machine.
