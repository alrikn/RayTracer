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

CONVERT = convert/convert_ppm.cpp
#tiny program to convert ppm to png, not used in the raytracer but needs to be done as sparate binary
CONVERT_NAME = ppm_to_png
CONVERT_OBJ = $(CONVERT:.cpp=.o)


OBJ = $(SRC:.cpp=.o)
CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
CXXFLAGS_CONVERT = -std=c++20 -w
CPPFLAGS = -I include

all: $(NAME) convert

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(CONVERT_OBJ): CXXFLAGS = $(CXXFLAGS_CONVERT)

$(CONVERT_NAME): $(CONVERT_OBJ)
	$(CXX) $(CXXFLAGS_CONVERT) -o $(CONVERT_NAME) $(CONVERT_OBJ)

convert: $(CONVERT_NAME)

clean:
	rm -f $(OBJ) $(CONVERT_OBJ)

fclean: clean
	rm -f $(NAME) $(CONVERT_NAME)

re: fclean all