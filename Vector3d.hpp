/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Vector3d
*/

#ifndef INCLUDED_VECTOR3D_HPP
    #define INCLUDED_VECTOR3D_HPP

#include <iostream>

class Vector3d
{
    private:
    protected:
    public:
        Vector3d(double x, double y, double z);
        ~Vector3d() = default;


        double x;
        double y;
        double z;

        double length() const;

        double dot(const Vector3d &other) const;

        /*operator overloading*/
        Vector3d& operator+=(const Vector3d& rhs);
        Vector3d& operator-=(const Vector3d& rhs);
        Vector3d& operator*=(const Vector3d& rhs);
        Vector3d& operator/=(const Vector3d& rhs);

        Vector3d& operator*=(double rhs);
        Vector3d& operator/=(double rhs);


};

Vector3d operator+(const Vector3d &lhs, const Vector3d &rhs);
Vector3d operator-(const Vector3d &lhs, const Vector3d &rhs);
Vector3d operator*(const Vector3d &lhs, const Vector3d &rhs);
Vector3d operator/(const Vector3d &lhs, const Vector3d &rhs);

Vector3d operator*(const Vector3d &lhs, double rhs);
Vector3d operator/(const Vector3d &lhs, double rhs);



#endif
