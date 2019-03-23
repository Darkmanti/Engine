#pragma once

#include <stdint.h>

class Collector
{
public:
	Collector();
	int8_t Build();
	int8_t Clear();
	~Collector();
};
