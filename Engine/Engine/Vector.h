#pragma once
#include <stdint.h>

class Vector
{
public:
	int64_t x, y, z;

	Vector(int64_t _x, int64_t _y, int64_t _z);
	Vector(int64_t _x, int64_t _y);
	Vector(int64_t _x);
	Vector();
};
