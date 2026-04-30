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
    auto it = COLOR_MAP.find(colorStr);
    if (it != COLOR_MAP.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Unknown color: " + colorStr);
    }
}

bool Parser::parseBool(const libconfig::Setting& setting)
{
    try {
        return static_cast<bool>(setting);
    } catch (const libconfig::SettingTypeException&) {
        throw std::runtime_error("Expected a boolean value");
    }
}
