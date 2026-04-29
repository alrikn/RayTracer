# Ray Tracer

Here is a current example of what it looks like:

![Example Render](docs/output.png)


## Description

This is an efficient raytracer written in C++.

Currently supports:
- Shapes
  - Spheres
  - Planes
- Lights
  - Specular
  - Directional
  - Ambient

## Performance

Here is a flame graph.


![Flame Graph](docs/flame.svg)

It can currently run 8000*4000 at 4 max depth in about 13 seconds on my machine. This is with no multithreading, so there is definitely room for improvement.

