#include "vector3D.h"

using namespace MVision;

Vector3D::Vector3D() : Vector2D(), z(0)
{
}

Vector3D::Vector3D(float X, float Y, float Z) : Vector2D(X, Y), z(Z)
{
}
