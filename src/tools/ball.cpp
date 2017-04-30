#include "ball.h"

using namespace MVision;

Ball::Ball(const Circle& position, float Confidence) :
    _positionInImage(position),
    _confidence(Confidence)
{

}

Ball::Ball(float X, float Y, float R, float Confidence) :
    _positionInImage(Circle(Vector2D(X, Y), R)),
    _confidence(Confidence)
{
}

Ball::Ball() :
    _positionInImage(Circle()),
    _confidence(0)
{
}
