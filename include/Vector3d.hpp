/*
** EPITECH PROJECT, 2026
** bootstrap_raytracer
** File description:
** Vector3d
*/

#ifndef INCLUDED_VECTOR3D_HPP
    #define INCLUDED_VECTOR3D_HPP

namespace Math
{
class Vector3d
{
    private:
    protected:
    public:
        Vector3d() = default;
        Vector3d(double x, double y, double z);
        ~Vector3d() = default;


        double x = 0;
        double y = 0;
        double z = 0;

        double length() const;

        //dot product of this vector with another vector
        double dot(const Vector3d &other) const;

        void translate(const Vector3d &translation);
        void rotateX(double angle);
        void rotateY(double angle);
        void rotateZ(double angle);

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

}

#endif
