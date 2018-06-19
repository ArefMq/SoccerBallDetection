#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>
#include "vector2D.h"

namespace MVision {
class Vector3D : public Vector2D
{
protected:
    float _z;

public:
    inline float z() const { return _z; }
    inline void setZ(float value) { _z = value; }
    inline float& mutable_z() { return _z; }

    Vector3D();
    Vector3D(float X, float Y, float Z=0);

    inline Vector3D& operator=(const Vector3D& other)
    {
        _x = other._x;
        _y = other._y;
        _z = other._z;
        return *this;
    }

    inline Vector3D operator+(const Vector3D& other) const
    {
        return Vector3D(_x + other._x, _y + other._y, _z + other._z);
    }

    inline Vector3D operator-(const Vector3D& other) const
    {
        return Vector3D(_x - other._x, _y - other._y, _z - other._z);
    }

    inline Vector3D operator-() const
    {
        return Vector3D(-_x, -_y, -_z);
    }

    inline float operator*(const Vector3D& other) const
    {
        return _x * other._x + _y * other._y + _z * other._z;
    }

    inline Vector3D operator*(const float& factor) const
    {
        return Vector3D(_x * factor, _y * factor, _z * factor);
    }

    inline Vector3D operator/(const float& factor) const
    {
        return Vector3D(_x / factor, _y / factor, _z / factor);
    }

    inline bool operator==(const Vector3D& other) const
    {
        return _x == other._x && _y == other._y && _z == other._z;
    }

    inline bool operator!=(const Vector3D& other) const
    {
        return _x != other._x || _y != other._y || _z != other._z;
    }

    inline float abs() const
    {
        return std::sqrt(_x * _x + _y * _y + _z * _z);
    }

    inline float sqr() const
    {
        return _x * _x + _y * _y + _z * _z;
    }

    inline bool operator <(const Vector3D &p) const
    {
        return _x < p._x || (_x == p._x && _y < p._y) || (_x == p._x && _y == p._y && _z < p._z);
    }
};
}

#endif // VECTOR3D_H
