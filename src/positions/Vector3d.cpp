/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Vector3d
*/

#include "Vector3d.hpp"
#include <cmath>

namespace Math {




Vector3d::Vector3d(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3d::length() const
{
    return std::sqrt((x * x) + (y * y) + (z * z)); //parenthesis is unecessary but it makes it more readable
}

double Vector3d::dot(const Vector3d &other) const
{
    return (x * other.x) + (y * other.y) + (z * other.z);
}




/*operator overloading*/
Vector3d operator+(const Vector3d &lhs, const Vector3d &rhs)
{
    return Vector3d(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vector3d operator-(const Vector3d &lhs, const Vector3d &rhs)
{
    return Vector3d(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vector3d operator*(const Vector3d &lhs, const Vector3d &rhs)
{
    return Vector3d(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

Vector3d operator/(const Vector3d &lhs, const Vector3d &rhs)
{
    return Vector3d(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

Vector3d operator*(const Vector3d &lhs, double rhs)
{
    return Vector3d(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vector3d operator/(const Vector3d &lhs, double rhs)
{
    return Vector3d(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

/* applying to self */

Vector3d& Vector3d::operator+=(const Vector3d& rhs)
{
    *this = *this + rhs;
    return *this;
}

Vector3d& Vector3d::operator-=(const Vector3d& rhs)
{
    *this = *this - rhs;
    return *this;
}

Vector3d& Vector3d::operator*=(const Vector3d& rhs)
{
    *this = *this * rhs;
    return *this;
}

Vector3d& Vector3d::operator/=(const Vector3d& rhs)
{
    *this = *this / rhs;
    return *this;
}

Vector3d& Vector3d::operator*=(double rhs)
{
    *this = *this * rhs;
    return *this;
}

Vector3d& Vector3d::operator/=(double rhs)
{
    *this = *this / rhs;
    return *this;
}
}
