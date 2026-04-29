/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** helper
*/

#include "Parser.hpp"

/*the helper functions */

double Parser::parseDouble(const libconfig::Setting& setting)
{
    double value = 0.0;
    //we need to be able to parse both integers and doubles, as if we try to parse a float when its an int, we crash
    try {
        value = setting;
    } catch (const libconfig::SettingTypeException&) {
        int intValue = setting;
        value = static_cast<double>(intValue);
    }
    return value;
}

Math::Vector3d Parser::parseVector3d(const libconfig::Setting& array)
{
    if (array.getLength() != 3 || !array.isArray()) {
        throw std::runtime_error("Expected an array of length 3 for Vector3d");
    }
    double x = parseDouble(array[0]);
    double y = parseDouble(array[1]);
    double z = parseDouble(array[2]);
    return Math::Vector3d(x, y, z);
}

Math::Point3d Parser::parsePoint3d(const libconfig::Setting& array)
{
    if (array.getLength() != 3 || !array.isArray()) {
        throw std::runtime_error("Expected an array of length 3 for Point3d");
    }
    double x = parseDouble(array[0]);
    double y = parseDouble(array[1]);
    double z = parseDouble(array[2]);
    return Math::Point3d(x, y, z);
}

Color Parser::parseColor(const libconfig::Setting& string)
{
    std::string colorStr = string.c_str();
    //TODO: MAKE A MAP
    if (colorStr == "red") return RED;
    if (colorStr == "green") return GREEN;
    if (colorStr == "blue") return BLUE;
    if (colorStr == "white") return WHITE;
    if (colorStr == "black") return BLACK;
    if (colorStr == "yellow") return YELLOW;
    // Add more colors as needed
    std::cerr << "Unknown color: " << colorStr << ". Defaulting to white." << std::endl;
    return WHITE; // default color
}
