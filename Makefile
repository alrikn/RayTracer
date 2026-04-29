##
## EPITECH PROJECT, 2026
## bootstrap_raytracer
## File description:
## Makefile
##

##
## EPITECH PROJECT, 2025
## nanotekspice
## File description:
## Makefile
##

NAME = raytracer
SRC = main.cpp	\
	src/positions/Vector3d.cpp	\
	src/positions/Point3d.cpp	\
	src/RayTracer/shapes/Rectangle.cpp	\
	src/RayTracer/shapes/Sphere.cpp	\
	src/RayTracer/shapes/Plane.cpp	\
	src/RayTracer/Camera.cpp	\
	src/RayTracer/Scene.cpp	\
	src/RayTracer/lights/DirectionalLight.cpp	\
	src/RayTracer/lights/AmbientLight.cpp	\
	src/RayTracer/lights/SpecularLight.cpp   \


OBJ = $(SRC:.cpp=.o)
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -O2 -fno-omit-frame-pointer
CPPFLAGS = -I include -I include/Shape -I include/Light -I include/Math

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

flame_check: re
	perf record -g --call-graph dwarf ./raytracer > output.ppm
	perf script > out.perf
	./graph_test/stackcollapse-perf.pl out.perf > out.folded
	./graph_test/flamegraph.pl --width 3000 out.folded > docs/flame.svg
	rm -f out.perf out.folded perf.data perf.data.old

clean:
	rm -f $(OBJ) $(CONVERT_OBJ)

fclean: clean
	rm -f $(NAME) $(CONVERT_NAME)

re: fclean all
