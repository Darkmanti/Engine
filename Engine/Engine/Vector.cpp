#include "Vector.h"

Vector::Vector(int64_t _x, int64_t _y, int64_t _z) : x(_x), y(_y), z(_z)
{
}

Vector::Vector(int64_t _x, int64_t _y) : x(_x), y(_y), z(0)
{
}

Vector::Vector(int64_t _x) : x(_x), y(0), z(0)
{
}

Vector::Vector() : x(0), y(0), z(0)
{
}
