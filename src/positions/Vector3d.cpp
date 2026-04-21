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

void Vector3d::translate(const Vector3d &translation)
{
    this->x += translation.x;
    this->y += translation.y;
    this->z += translation.z;
}

void Vector3d::rotateX(double angle)
{
    double cos_angle = std::cos(angle);
    double sin_angle = std::sin(angle);
    double y = this->y * cos_angle - this->z * sin_angle;
    double z = this->y * sin_angle + this->z * cos_angle;
    this->y = y;
    this->z = z;
}

void Vector3d::rotateY(double angle)
{
    double cos_angle = std::cos(angle);
    double sin_angle = std::sin(angle);
    double x = this->x * cos_angle + this->z * sin_angle;
    double z = -this->x * sin_angle + this->z * cos_angle;
    this->x = x;
    this->z = z;
}

void Vector3d::rotateZ(double angle)
{
    double cos_angle = std::cos(angle);
    double sin_angle = std::sin(angle);
    double x = this->x * cos_angle - this->y * sin_angle;
    double y = this->x * sin_angle + this->y * cos_angle;
    this->x = x;
    this->y = y;
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
