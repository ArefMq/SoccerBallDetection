#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

namespace MVision {
class Vector2D
{
protected:
    float _x, _y;

public:
    inline float x() const { return _x; }
    inline float y() const { return _y; }

    inline void setX(float value) { _x = value; }
    inline void setY(float value) { _y = value; }

    inline float& mutable_x() { return _x; }
    inline float& mutable_y() { return _y; }

    Vector2D();
    Vector2D(float X, float Y);

    inline Vector2D& operator=(const Vector2D& other)
    {
        _x = other._x;
        _y = other._y;
        return *this;
    }

    inline Vector2D operator+(const Vector2D& other) const
    {
        return Vector2D(_x + other._x, _y + other._y);
    }

    inline Vector2D operator-(const Vector2D& other) const
    {
        return Vector2D(_x - other._x, _y - other._y);
    }

    inline Vector2D operator-() const
    {
        return Vector2D(-_x, -_y);
    }

    inline float operator*(const Vector2D& other) const
    {
        return _x * other._x + _y * other._y;
    }

    inline Vector2D operator*(const float& factor) const
    {
        return Vector2D(_x * factor, _y * factor);
    }

    inline Vector2D operator/(const float& factor) const
    {
        return Vector2D(_x / factor, _y / factor);
    }

    inline bool operator==(const Vector2D& other) const
    {
        return _x == other._x && _y == other._y;
    }

    inline bool operator!=(const Vector2D& other) const
    {
        return _x != other._x || _y != other._y;
    }

    inline float abs() const
    {
        return std::sqrt(((float)_x) * _x + ((float)_y) * _y);
    }

    inline float sqr() const
    {
        return _x * _x + _y * _y;
    }

    inline bool operator <(const Vector2D &p) const
    {
        return _x < p._x || (_x == p._x && _y < p._y);
    }
};
}
#endif // VECTOR2D_H
