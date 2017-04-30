#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include "vector2D.h"

namespace MVision {
class Vector3D : public Vector2D
{
public:
    Vector3D();
    Vector3D(float X, float Y, float Z=0);

    // [FIXME] : fix here
    float z;

    inline Vector3D& operator=(const Vector3D& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    /** Addition of another vector to this one.
      *\param other The other vector that will be added to this one
      *\return A new object that contains the result of the calculation.
      */
    inline Vector3D operator+(const Vector3D& other) const
    {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    /** Subtraction of another vector to this one.
      *\param other The other vector that will be added to this one
      *\return A new object that contains the result of the calculation.
      */
    inline Vector3D operator-(const Vector3D& other) const
    {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    /** Negation of this vector.
      *\return A new object that contains the result of the calculation.
      */
    inline Vector3D operator-() const
    {
        return Vector3D(-x, -y, -z);
    }

    /** Inner product of this vector and another one.
      *\param other The other vector this one will be multiplied by
      *\return The inner product.
      */
    inline float operator*(const Vector3D& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    /** Multiplication of this vector by a factor.
      *\param factor The factor this vector is multiplied by
      *\return A new object that contains the result of the calculation.
      */
    inline Vector3D operator*(const float& factor) const
    {
        return Vector3D(x * factor, y * factor, z * factor);
    }

    /** Division of this vector by a factor.
      *
      *\param factor The factor this vector is divided by
      *\return A new object that contains the result of the calculation.
      */
    inline Vector3D operator/(const float& factor) const
    {
        return Vector3D(x / factor, y / factor, z / factor);
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one
      *\return Whether the two vectors are equal.
      */
    inline bool operator==(const Vector3D& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one.
      *\return Whether the two vectors are unequal.
      */
    inline bool operator!=(const Vector3D& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    /** Calculation of the length of this vector.
      *\return The length.
      */
    inline float abs() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    /**
      * Calculation of the square of the vector.
      * \return The square
      */
    inline float sqr() const
    {
        return x * x + y * y + z * z;
    }

    inline bool operator <(const Vector3D &p) const
    {
        return x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z);
    }
};
}

#endif // VECTOR3D_H
