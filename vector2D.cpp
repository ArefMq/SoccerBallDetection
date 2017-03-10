#include "vector2D.h"

Vector2D::Vector2D() : x(0), y(0)
{
}

Vector2D::Vector2D(float X, float Y) : x(X), y(Y)
{
}

Vector3D::Vector3D() : Vector2D(), z(0)
{
}

Vector3D::Vector3D(float X, float Y, float Z) : Vector2D(X, Y), z(Z)
{
}
