#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector2D.h"

namespace MVision {
class Circle
{
protected:
    Vector2D _translation;
    float _radious;

public:
    inline const Vector2D& translation() const { return _translation; }
    inline float radious() const { return _radious; }

    inline void setTranslation(const Vector2D& value) { _translation = value; }
    inline void setRadious(float value) { _radious = value; }

    inline Vector2D& mutable_translation() { return _translation; }
    inline float& mutable_radious() { return _radious; }

    Circle();
    Circle(const Vector2D& Translation, float Radious);

    inline Circle& operator=(const Circle& other)
    {
        _translation = other._translation;
        _radious = other._radious;
        return *this;
    }

    inline bool operator==(const Circle& other) const
    {
        return _translation == other._translation && _radious == other._radious;
    }

    inline bool operator!=(const Circle& other) const
    {
        return _translation != other._translation || _radious != other._radious;
    }
};
}

#endif // CIRCLE_H
