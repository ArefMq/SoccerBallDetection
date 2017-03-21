#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D
{
public:
    Vector2D();
    Vector2D(float X, float Y);

    // [FIXME] : fix here
    float x, y;

    inline Vector2D& operator=(const Vector2D& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    /** Addition of another vector to this one.
      *\param other The other vector that will be added to this one
      *\return A new object that contains the result of the calculation.
      */
    inline Vector2D operator+(const Vector2D& other) const
    {
        return Vector2D(x + other.x, y + other.y);
    }

    /** Subtraction of another vector to this one.
      *\param other The other vector that will be added to this one
      *\return A new object that contains the result of the calculation.
      */
    inline Vector2D operator-(const Vector2D& other) const
    {
        return Vector2D(x - other.x, y - other.y);
    }

    /** Negation of this vector.
      *\return A new object that contains the result of the calculation.
      */
    inline Vector2D operator-() const
    {
        return Vector2D(-x, -y);
    }

    /** Inner product of this vector and another one.
      *\param other The other vector this one will be multiplied by
      *\return The inner product.
      */
    inline float operator*(const Vector2D& other) const
    {
        return x * other.x + y * other.y;
    }

    /** Multiplication of this vector by a factor.
      *\param factor The factor this vector is multiplied by
      *\return A new object that contains the result of the calculation.
      */
    inline Vector2D operator*(const float& factor) const
    {
        return Vector2D(x * factor, y * factor);
    }

    /** Division of this vector by a factor.
      *
      *\param factor The factor this vector is divided by
      *\return A new object that contains the result of the calculation.
      */
    inline Vector2D operator/(const float& factor) const
    {
        return Vector2D(x / factor, y / factor);
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one
      *\return Whether the two vectors are equal.
      */
    inline bool operator==(const Vector2D& other) const
    {
        return x == other.x && y == other.y;
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one.
      *\return Whether the two vectors are unequal.
      */
    inline bool operator!=(const Vector2D& other) const
    {
        return x != other.x || y != other.y;
    }

    /** Calculation of the length of this vector.
      *\return The length.
      */
    inline float abs() const
    {
        return std::sqrt(((float)x) * x + ((float)y) * y);
    }

    /**
      * Calculation of the square of the vector.
      * \return The square
      */
    inline float sqr() const
    {
        return x * x + y * y;
    }

    inline bool operator <(const Vector2D &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }
};

#endif // VECTOR2D_H
