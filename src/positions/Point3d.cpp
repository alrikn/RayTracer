/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Point3d
*/

#include "Point3d.hpp"
#include "Vector3d.hpp"



namespace Math {

Vector3d operator+(const Point3d &lhs, const Point3d &rhs)
{
    return Vector3d(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector3d operator-(const Point3d &lhs, const Point3d &rhs)
{
    return Vector3d(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Point3d operator+(const Point3d &lhs, const Vector3d &rhs)
{
    return Point3d(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Point3d operator-(const Point3d &lhs, const Vector3d &rhs)
{
    return Point3d(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

}