/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** Parser
*/

#ifndef INCLUDED_PARSER_HPP
    #define INCLUDED_PARSER_HPP

#include "Scene.hpp"

/*
SHAPES:
    sphere:
    - origin (point3d) OBLIGATORY
    - radius (double) OBLIGATORY
    - color (color) OPTIONAL

    plane:
    - normal (vector3d) OBLIGATORY
    - point (point3d) OBLIGATORY //might be optional in future haven't decided
    - color (color) OPTIONAL

    rectangle:
    - origin (point3d) OBLIGATORY
    - bottom_side (vector3d) OBLIGATORY
    - left_side (vector3d) OBLIGATORY
    - color (color) OPTIONAL

LIGHTS:
    ambient:
    - brightness (double) OBLIGATORY
    - color (vector3d) OPTIONAL //for the lights its a vector3d because i want the user to have more control

    directional:
    - direction (vector3d) OBLIGATORY
    - brightness (double) OBLIGATORY
    - color (vector3d) OPTIONAL

    specular:
    - position (point3d) OBLIGATORY
    - brightness (double) OBLIGATORY
    - color (vector3d) OPTIONAL
*/


class Parser
{
    private:
        RayTracer::Scene scene; //the scene that will be rendered. the parser will fill this scene with objects and lights based on the input file
    public:
        Parser();

        ~Parser() = default;

};


/*

*/

#endif
