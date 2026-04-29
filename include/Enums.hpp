/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Enums
*/

#ifndef INCLUDED_ENUMS_HPP
    #define INCLUDED_ENUMS_HPP

//these be a few colors shapes can be at (only main colors)
enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    CYAN,
    WHITE,
    BLACK,
};


/*
enum of all the shapes. this will be used for the parser to know which shape to create.
*/
enum ShapeType {
    SPHERE,
    PLANE,
    RECTANGLE,
    //CAMERA, for now the camera will be hardcoded, but eventually it will have to be parsed
};

//for making the lights
enum LightType {
    AMBIENT,
    DIRECTIONAL,
    SPECULAR
};

//for knowing what each object/light takes in as input for the parser
enum MathType {
    VECTOR3D,
    POINT3D,
    DOUBLE, //just a normal double, but sometimes is needed
};


#endif
