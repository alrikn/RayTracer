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
	src/raytracer/shapes/Rectangle.cpp	\
	src/raytracer/shapes/Sphere.cpp	\
	src/raytracer/camera.cpp	\



OBJ = $(SRC:.cpp=.o)
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
CPPFLAGS = -I include

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all