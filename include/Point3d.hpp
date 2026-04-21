/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Point3d
*/

#ifndef INCLUDED_POINT3D_HPP
    #define INCLUDED_POINT3D_HPP

namespace Math
{

class Vector3d; // Forward declaration of Vector3d

class Point3d
{
    private:
    protected:
    public:
        Point3d();
        Point3d(double x, double y, double z) : x(x), y(y), z(z) {};
        ~Point3d() = default;

        double x = 0;
        double y = 0;
        double z = 0;

};

/*operator overloading*/
Point3d operator+(const Point3d &lhs, const Vector3d &rhs);
Point3d operator-(const Point3d &lhs, const Vector3d &rhs);

}

#endif
