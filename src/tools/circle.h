#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector2D.h"

namespace MVision {
class Circle
{
//private:
public: // [FIXME] : fix here
    Vector2D _translation;
    float _radious;

public:
    Circle();
    Circle(const Vector2D& Translation, float Radious);

    inline Circle& operator=(const Circle& other)
    {
        _translation = other._translation;
        _radious = other._radious;
        return *this;
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one
      *\return Whether the two vectors are equal.
      */
    inline bool operator==(const Circle& other) const
    {
        return _translation == other._translation && _radious == other._radious;
    }

    /** Comparison of another vector with this one.
      *\param other The other vector that will be compared to this one.
      *\return Whether the two vectors are unequal.
      */
    inline bool operator!=(const Circle& other) const
    {
        return _translation != other._translation || _radious != other._radious;
    }
};
}

#endif // CIRCLE_H
