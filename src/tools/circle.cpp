#include "circle.h"

using namespace MVision;

Circle::Circle() : _translation(Vector2D()), _radious(0)
{
}

Circle::Circle(const Vector2D& Translation, float Radious) :
        _translation(Translation),
        _radious(Radious)
{
}
