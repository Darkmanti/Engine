#pragma once

#include <stdint.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "GameObject.h"

namespace Importer
{
	uint32_t Obj(const char *fileName, GameObject &go);
	uint32_t Bmp(const char *fileName, GameObject &go);
	uint32_t Png(const char *fileName, GameObject &go);
};
